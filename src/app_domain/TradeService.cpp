#include "Precompiled.h"
#include "TradeService.h"

namespace app_domain
{
    TradeService::TradeService(ItemService& itemService, CharacterService& characterService,
        InventoryService& inventoryService)
        : m_itemService(itemService), m_characterService(characterService), m_inventoryService(inventoryService)
    {
    }

    tl::expected<TradeContext, TradeError> TradeService::MakeContext(const std::string& buyerCharacterId,
        const std::string& sellerCharacterId) const
    {
        if (buyerCharacterId == sellerCharacterId)
            return tl::unexpected(TradeError::TradeWithSelfNotAllowed);

        auto buyerResult = m_characterService.GetCharacterById(buyerCharacterId);
        auto sellerResult = m_characterService.GetCharacterById(sellerCharacterId);
        if (!buyerResult || !sellerResult)
            return tl::unexpected(TradeError::CharacterNotFound);

        auto& buyerCharacter = buyerResult.value().get();
        auto& sellerCharacter = sellerResult.value().get();

        auto buyerInventoryResult = m_inventoryService.GetInventoryById(buyerCharacter.InventoryId);
        auto sellerInventoryResult = m_inventoryService.GetInventoryById(sellerCharacter.InventoryId);
        if (!buyerInventoryResult || !sellerInventoryResult)
            return tl::unexpected(TradeError::InventoryNotFound);

        return TradeContext{
            buyerResult.value().get(),
            sellerResult.value().get(),
            buyerInventoryResult.value().get(),
            sellerInventoryResult.value().get()
        };
    }

    bool TradeService::IsItemTradable(const Item& item) const
    {
        return !item.IsStoryItem;
    }

    tl::expected<InventoryItemDetails, TradeError> TradeService::CanTradeItem(const std::string& buyerCharacterId,
        const std::string& sellerCharacterId,
        std::size_t itemIndex,
        std::uint32_t count) const
    {
        auto contextResult = MakeContext(buyerCharacterId, sellerCharacterId);
        if (!contextResult)
            return tl::unexpected(contextResult.error());

        return CanTradeItemInternal(contextResult.value(), itemIndex, count);
    }

    tl::expected<void, TradeError> TradeService::TradeItem(const std::string& buyerCharacterId,
        const std::string& sellerCharacterId,
        std::size_t itemIndex,
        std::uint32_t count) const
    {
        auto contextResult = MakeContext(buyerCharacterId, sellerCharacterId);
        if (!contextResult)
            return tl::unexpected(contextResult.error());

        return TradeItemInternal(contextResult.value(), itemIndex, count);
    }

    tl::expected<InventoryItemDetails, TradeError> TradeService::CanTradeItemInternal(const TradeContext& context,
        std::size_t itemIndex,
        std::uint32_t count) const
    {
        const auto& sellerInventoryItemResult = m_inventoryService.GetItemDetails(context.SellerInventory.Id, itemIndex);
        if (!sellerInventoryItemResult)
        {
            switch (sellerInventoryItemResult.error())
            {
            case InventoryError::NotFound:         
                return tl::unexpected(TradeError::InventoryNotFound);
            case InventoryError::IndexOutOfRange:  
                return tl::unexpected(TradeError::InventoryItemNotFound);
            case InventoryError::ItemNotFound:     
                return tl::unexpected(TradeError::ItemNotFound);
            }
        }

        const auto& sellerInventoryItem = sellerInventoryItemResult.value();
        const auto& item = sellerInventoryItem.GetItem();

        if (!IsItemTradable(item))
            return tl::unexpected(TradeError::ItemNotTradable);

        if (count == TradeService::TradeAll)
            count = sellerInventoryItem.GetCount();

        if (sellerInventoryItem.GetCount() < count)
            return tl::unexpected(TradeError::InvalidAmount);

        const uint32_t totalValue = item.Value * count;

        if (context.BuyerInventory.CurrentMoney < totalValue)
            return tl::unexpected(TradeError::NotEnoughMoney);

        return sellerInventoryItem;
    }

    tl::expected<void, TradeError> TradeService::TradeItemInternal(const TradeContext& context,
        std::size_t itemIndex,
        std::uint32_t count) const
    {
        auto canTradeResult = CanTradeItemInternal(context, itemIndex, count);
        if (!canTradeResult)
            return tl::unexpected(canTradeResult.error());

        const auto& sellerInventoryItem = canTradeResult.value();
        const auto& item = sellerInventoryItem.GetItem();

        if (count == TradeService::TradeAll)
            count = sellerInventoryItem.GetCount();

        const uint32_t totalValue = item.Value * count;

        if (!m_inventoryService.TransferMoney(context.BuyerInventory.Id, context.SellerInventory.Id, totalValue))
            return tl::unexpected(TradeError::TransferFailed);

        if (!m_inventoryService.TransferItem(context.SellerInventory.Id, context.BuyerInventory.Id, itemIndex, count))
            return tl::unexpected(TradeError::TransferFailed);

        return {};
    }
}