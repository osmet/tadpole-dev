#include "Precompiled.h"
#include "TradeService.h"

namespace app_domain
{
    TradeError TradeService::ToTradeError(InventoryError inventoryError)
    {
        switch (inventoryError)
        {
        case InventoryError::NotFound:
            return TradeError::InventoryNotFound;
        case InventoryError::NotEnoughMoney:
            return TradeError::NotEnoughMoney;
        case InventoryError::InvalidAmount:
            return TradeError::InvalidAmount;
        case InventoryError::IndexOutOfRange:
            return TradeError::InventoryItemNotFound;
        case InventoryError::ItemNotFound:
            return TradeError::ItemNotFound;
        default:
            return TradeError::TransferFailed;
        }
    }

    TradeService::TradeService(ItemService& itemService, CharacterService& characterService,
        InventoryService& inventoryService)
        : m_itemService(itemService), m_characterService(characterService), m_inventoryService(inventoryService)
    {
    }

    lang::Expected<TradeContext, TradeError> 
        TradeService::MakeContext(const std::string& fromCharacterId, const std::string& toCharacterId) const
    {
        if (fromCharacterId == toCharacterId)
            return lang::Unexpected(TradeError::TradeWithSelfNotAllowed);

        auto fromResult = m_characterService.GetCharacterById(fromCharacterId);
        auto toResult = m_characterService.GetCharacterById(toCharacterId);
        if (!fromResult || !toResult)
            return lang::Unexpected(TradeError::CharacterNotFound);

        auto& fromCharacter = fromResult.value().get();
        auto& toCharacter = toResult.value().get();

        auto fromInventoryResult = m_inventoryService.GetInventoryById(fromCharacter.InventoryId);
        auto toInventoryResult = m_inventoryService.GetInventoryById(toCharacter.InventoryId);
        if (!fromInventoryResult || !toInventoryResult)
            return lang::Unexpected(TradeError::InventoryNotFound);

        return TradeContext{
            .FromCharacter = fromCharacter,
            .ToCharacter = toCharacter,
            .FromInventory = fromInventoryResult.value().get(),
            .ToInventory = toInventoryResult.value().get()
        };
    }

    bool TradeService::IsItemTradable(const Item& item) const
    {
        return !item.IsStoryItem;
    }

    lang::Expected<void, TradeError> 
        TradeService::CanTradeItem(const std::string& fromCharacterId, const std::string& toCharacterId,
            std::size_t itemIndex, std::uint32_t count) const
    {
        auto contextResult = MakeContext(fromCharacterId, toCharacterId);
        if (!contextResult)
            return lang::Unexpected(contextResult.error());

        auto result = CanTradeItemInternal(contextResult.value(), itemIndex, count);
        if (!result)
            return lang::Unexpected(result.error());

        return {};
    }

    lang::Expected<void, TradeError> 
        TradeService::TradeItem(const std::string& fromCharacterId, const std::string& toCharacterId,
            std::size_t itemIndex, std::uint32_t count) const
    {
        auto contextResult = MakeContext(fromCharacterId, toCharacterId);
        if (!contextResult)
            return lang::Unexpected(contextResult.error());

        return TradeItemInternal(contextResult.value(), itemIndex, count);
    }

    lang::Expected<InventoryItemDetails, TradeError> 
        TradeService::CanTradeItemInternal(const TradeContext& context, std::size_t itemIndex,
            std::uint32_t count) const
    {
        const auto& fromInventoryItemResult = m_inventoryService.GetItemDetails(context.FromInventory.Id, itemIndex);
        if (!fromInventoryItemResult)
            return lang::Unexpected(app_domain::TradeService::ToTradeError(fromInventoryItemResult.error()));

        const auto& fromInventoryItem = fromInventoryItemResult.value();
        const auto& item = fromInventoryItem.GetItem();

        if (!IsItemTradable(item))
            return lang::Unexpected(TradeError::ItemNotTradable);

        if (count == TradeService::TradeAll)
            count = fromInventoryItem.GetCount();

        if (fromInventoryItem.GetCount() < count)
            return lang::Unexpected(TradeError::InvalidAmount);

        const uint32_t totalValue = item.Value * count;

        if (context.ToInventory.CurrentMoney < totalValue)
            return lang::Unexpected(TradeError::NotEnoughMoney);

        return fromInventoryItem;
    }

    lang::Expected<void, TradeError> 
        TradeService::TradeItemInternal(const TradeContext& context, std::size_t itemIndex,
            std::uint32_t count) const
    {
        auto canTradeResult = CanTradeItemInternal(context, itemIndex, count);
        if (!canTradeResult)
            return lang::Unexpected(canTradeResult.error());

        const auto& fromInventoryItem = canTradeResult.value();
        const auto& item = fromInventoryItem.GetItem();

        if (count == TradeService::TradeAll)
            count = fromInventoryItem.GetCount();

        const uint32_t totalValue = item.Value * count;

        if (!m_inventoryService.TransferMoney(context.ToInventory.Id, context.FromInventory.Id, totalValue))
            return lang::Unexpected(TradeError::TransferFailed);

        if (!m_inventoryService.TransferItem(context.FromInventory.Id, context.ToInventory.Id, itemIndex, count))
            return lang::Unexpected(TradeError::TransferFailed);

        return {};
    }
}