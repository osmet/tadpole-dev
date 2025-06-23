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
        auto sellerInventoryItemResult = m_inventoryService.GetItem(context.SellerInventory.Id, itemIndex);
        if (!sellerInventoryItemResult)
            return tl::unexpected(TradeError::InventoryItemNotFound);

        const auto& sellerInventoryItem = sellerInventoryItemResult.value().get();

        auto itemResult = m_itemService.GetItemById(sellerInventoryItem.ItemId);
        if (!itemResult)
            return tl::unexpected(TradeError::ItemNotFound);

        const auto& item = itemResult.value().get();

        if (item.IsStoryItem)
            return tl::unexpected(TradeError::ItemNotTradable);

        if (context.BuyerInventory.CurrentMoney < item.Value)
            return tl::unexpected(TradeError::NotEnoughMoney);

        return {};
    }

    tl::expected<void, TradeError> TradeService::TradeItemInternal(const TradeContext& context,
        std::size_t itemIndex) const
    {
        auto canTrade = CanTradeItemInternal(context, itemIndex);
        if (!canTrade)
            return tl::unexpected(canTrade.error());

        const auto& sellerInventoryItem = m_inventoryService.GetItem(context.SellerInventory.Id, itemIndex).value().get();
        const auto& item = m_itemService.GetItemById(sellerInventoryItem.ItemId).value().get();

        const auto itemValue = item.Value;

        if (!m_inventoryService.TransferMoney(context.BuyerInventory.Id, context.SellerInventory.Id, itemValue))
            return tl::unexpected(TradeError::TransferFailed);

        if (!m_inventoryService.TransferItem(context.SellerInventory.Id, context.BuyerInventory.Id, itemIndex))
            return tl::unexpected(TradeError::TransferFailed);

        return {};
    }
}