#pragma once

#include "CharacterService.h"
#include "InventoryService.h"
#include "ItemService.h"

namespace app_domain
{
    enum class TradeError : std::int8_t
    {
        CharacterNotFound = 0,
        InventoryNotFound = 1,
        InventoryItemNotFound = 2,
        ItemNotFound = 3,
        NotEnoughMoney = 4,
        TransferFailed = 5,
    };

    class TradeService
    {
    public:
        TradeService(InventoryService& inventoryService, CharacterService& characterService, ItemService& itemService);

        tl::expected<void, TradeError> TradeItem(const std::string& buyerCharacterId,
            const std::string& sellerCharacterId,
            std::size_t itemIndex);

    private:
        InventoryService& m_inventoryService;
        CharacterService& m_characterService;
        ItemService& m_itemService;
    };
}