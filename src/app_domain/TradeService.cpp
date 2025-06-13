#include "Precompiled.h"
#include "TradeService.h"

namespace app_domain
{
    TradeService::TradeService(InventoryService& inventoryService, CharacterService& characterService, ItemService& itemService)
        : m_inventoryService(inventoryService), m_characterService(characterService), m_itemService(itemService)
    {
    }

    tl::expected<void, TradeError> TradeService::TradeItem(const std::string& buyerCharacterId,
        const std::string& sellerCharacterId,
        std::size_t itemIndex)
    {
        auto buyerResult = m_characterService.GetCharacterById(buyerCharacterId);
        auto sellerResult = m_characterService.GetCharacterById(sellerCharacterId);
        if (!buyerResult || !sellerResult)
            return tl::unexpected(TradeError::CharacterNotFound);

        const auto& buyerCharacter = buyerResult.value().get();
        const auto& sellerCharacter = sellerResult.value().get();

        const auto& buyerInventoryId = buyerCharacter.InventoryId;
        const auto& sellerInventoryId = sellerCharacter.InventoryId;

        auto buyerInventoryResult = m_inventoryService.GetInventoryById(buyerInventoryId);
        auto sellerInventoryResult = m_inventoryService.GetInventoryById(sellerInventoryId);
        if (!buyerInventoryResult || !sellerInventoryResult)
            return tl::unexpected(TradeError::InventoryNotFound);

        const auto& buyerInventory = buyerInventoryResult.value().get();

        auto sellerInventoryItemResult = m_inventoryService.GetItemByIndex(sellerInventoryId, itemIndex);
        if (!sellerInventoryItemResult)
            return tl::unexpected(TradeError::InventoryItemNotFound);

        const auto& sellerInventoryItem = sellerInventoryItemResult.value().get();

        auto itemResult = m_itemService.GetItemById(sellerInventoryItem.ItemId);
        if (!itemResult)
            return tl::unexpected(TradeError::ItemNotFound);

        const auto& item = itemResult.value().get();
        const auto itemValue = item.Value;

        if (buyerInventory.CurrentMoney < itemValue)
            return tl::unexpected(TradeError::NotEnoughMoney);

        if (!m_inventoryService.TransferMoney(buyerInventoryId, sellerInventoryId, itemValue))
            return tl::unexpected(TradeError::TransferFailed);

        if (!m_inventoryService.TransferItemByIndex(sellerInventoryId, buyerInventoryId, itemIndex))
            return tl::unexpected(TradeError::TransferFailed);

        return {};
    }
}