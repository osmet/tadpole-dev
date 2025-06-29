#include "Precompiled.h"
#include "TradeService.h"

namespace app_domain
{
    TradeService::TradeService(ItemService& itemService, CharacterService& characterService,
        InventoryService& inventoryService)
        : m_itemService(itemService), m_characterService(characterService), m_inventoryService(inventoryService)
    {
    }

    tl::expected<TradeContext, TradeError> 
        TradeService::MakeContext(const std::string& fromCharacterId, const std::string& toCharacterId) const
    {
        if (fromCharacterId == toCharacterId)
            return tl::unexpected(TradeError::TradeWithSelfNotAllowed);

        auto fromResult = m_characterService.GetCharacterById(fromCharacterId);
        auto toResult = m_characterService.GetCharacterById(toCharacterId);
        if (!fromResult || !toResult)
            return tl::unexpected(TradeError::CharacterNotFound);

        auto& fromCharacter = fromResult.value().get();
        auto& toCharacter = toResult.value().get();

        auto fromInventoryResult = m_inventoryService.GetInventoryById(fromCharacter.InventoryId);
        auto toInventoryResult = m_inventoryService.GetInventoryById(toCharacter.InventoryId);
        if (!fromInventoryResult || !toInventoryResult)
            return tl::unexpected(TradeError::InventoryNotFound);

        return TradeContext{
            fromCharacter,
            toCharacter,
            fromInventoryResult.value().get(),
            toInventoryResult.value().get()
        };
    }

    bool TradeService::IsItemTradable(const Item& item) const
    {
        return !item.IsStoryItem;
    }

    tl::expected<InventoryItemDetails, TradeError> 
        TradeService::CanTradeItem(const std::string& fromCharacterId, const std::string& toCharacterId,
            std::size_t itemIndex, std::uint32_t count) const
    {
        auto contextResult = MakeContext(fromCharacterId, toCharacterId);
        if (!contextResult)
            return tl::unexpected(contextResult.error());

        return CanTradeItemInternal(contextResult.value(), itemIndex, count);
    }

    tl::expected<void, TradeError> 
        TradeService::TradeItem(const std::string& fromCharacterId, const std::string& toCharacterId,
            std::size_t itemIndex, std::uint32_t count) const
    {
        auto contextResult = MakeContext(fromCharacterId, toCharacterId);
        if (!contextResult)
            return tl::unexpected(contextResult.error());

        return TradeItemInternal(contextResult.value(), itemIndex, count);
    }

    tl::expected<InventoryItemDetails, TradeError> 
        TradeService::CanTradeItemInternal(const TradeContext& context, std::size_t itemIndex,
            std::uint32_t count) const
    {
        const auto& fromInventoryItemResult = m_inventoryService.GetItemDetails(context.FromInventory.Id, itemIndex);
        if (!fromInventoryItemResult)
        {
            switch (fromInventoryItemResult.error())
            {
            case InventoryError::NotFound:
                return tl::unexpected(TradeError::InventoryNotFound);
            case InventoryError::IndexOutOfRange:
                return tl::unexpected(TradeError::InventoryItemNotFound);
            case InventoryError::ItemNotFound:
                return tl::unexpected(TradeError::ItemNotFound);
            }
        }

        const auto& fromInventoryItem = fromInventoryItemResult.value();
        const auto& item = fromInventoryItem.GetItem();

        if (!IsItemTradable(item))
            return tl::unexpected(TradeError::ItemNotTradable);

        if (count == TradeService::TradeAll)
            count = fromInventoryItem.GetCount();

        if (fromInventoryItem.GetCount() < count)
            return tl::unexpected(TradeError::InvalidAmount);

        const uint32_t totalValue = item.Value * count;

        if (context.ToInventory.CurrentMoney < totalValue)
            return tl::unexpected(TradeError::NotEnoughMoney);

        return fromInventoryItem;
    }

    tl::expected<void, TradeError> 
        TradeService::TradeItemInternal(const TradeContext& context, std::size_t itemIndex,
            std::uint32_t count) const
    {
        auto canTradeResult = CanTradeItemInternal(context, itemIndex, count);
        if (!canTradeResult)
            return tl::unexpected(canTradeResult.error());

        const auto& fromInventoryItem = canTradeResult.value();
        const auto& item = fromInventoryItem.GetItem();

        if (count == TradeService::TradeAll)
            count = fromInventoryItem.GetCount();

        const uint32_t totalValue = item.Value * count;

        if (!m_inventoryService.TransferMoney(context.ToInventory.Id, context.FromInventory.Id, totalValue))
            return tl::unexpected(TradeError::TransferFailed);

        if (!m_inventoryService.TransferItem(context.FromInventory.Id, context.ToInventory.Id, itemIndex, count))
            return tl::unexpected(TradeError::TransferFailed);

        return {};
    }
}