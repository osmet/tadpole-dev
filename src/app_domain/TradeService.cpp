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

    tl::expected<void, TradeError> TradeService::CanTradeItem(const std::string& buyerCharacterId,
        const std::string& sellerCharacterId,
        std::size_t itemIndex) const
    {
        auto contextResult = MakeContext(buyerCharacterId, sellerCharacterId);
        if (!contextResult)
            return tl::unexpected(contextResult.error());

        return CanTradeItemInternal(contextResult.value(), itemIndex);
    }

    tl::expected<void, TradeError> TradeService::TradeItem(const std::string& buyerCharacterId,
        const std::string& sellerCharacterId,
        std::size_t itemIndex) const
    {
        auto contextResult = MakeContext(buyerCharacterId, sellerCharacterId);
        if (!contextResult)
            return tl::unexpected(contextResult.error());

        return TradeItemInternal(contextResult.value(), itemIndex);
    }

    tl::expected<void, TradeError> TradeService::CanTradeItemInternal(const TradeContext& context,
        std::size_t itemIndex) const
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

        const uint32_t itemTotalValue = sellerInventoryItemResult.value().GetTotalValue();

        if (context.BuyerInventory.CurrentMoney < itemTotalValue)
            return tl::unexpected(TradeError::NotEnoughMoney);

        return {};
    }

    tl::expected<void, TradeError> TradeService::TradeItemInternal(const TradeContext& context,
        std::size_t itemIndex) const
    {
        auto canTradeResult = CanTradeItemInternal(context, itemIndex);
        if (!canTradeResult)
            return tl::unexpected(canTradeResult.error());

        const auto& sellerInventoryItemResult = m_inventoryService.GetItemDetails(context.SellerInventory.Id, itemIndex);

        const uint32_t itemTotalValue = sellerInventoryItemResult.value().GetTotalValue();

        if (!m_inventoryService.TransferMoney(context.BuyerInventory.Id, context.SellerInventory.Id, itemTotalValue))
            return tl::unexpected(TradeError::TransferFailed);

        if (!m_inventoryService.TransferItem(context.SellerInventory.Id, context.BuyerInventory.Id, itemIndex))
            return tl::unexpected(TradeError::TransferFailed);

        return {};
    }
}