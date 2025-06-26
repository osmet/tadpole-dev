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
        ItemNotTradable = 4,
        NotEnoughMoney = 5,
        TransferFailed = 6,
        TradeWithSelfNotAllowed = 7
    };

    struct TradeContext
    {
        const app_domain::Character& BuyerCharacter;
        const app_domain::Character& SellerCharacter;
        const app_domain::Inventory& BuyerInventory;
        const app_domain::Inventory& SellerInventory;
    };

    class TradeService
    {
    public:
        TradeService(ItemService& itemService, CharacterService& characterService,
            InventoryService& inventoryService);

        tl::expected<TradeContext, TradeError> MakeContext(const std::string& buyerCharacterId,
            const std::string& sellerCharacterId) const;

        bool IsItemTradable(const Item& item) const;

        tl::expected<void, TradeError> CanTradeItem(const std::string& buyerCharacterId,
            const std::string& sellerCharacterId,
            std::size_t itemIndex) const;

        tl::expected<void, TradeError> TradeItem(const std::string& buyerCharacterId,
            const std::string& sellerCharacterId,
            std::size_t itemIndex) const;

    private:
        tl::expected<void, TradeError> CanTradeItemInternal(const TradeContext& context, 
            std::size_t itemIndex) const;
        
        tl::expected<void, TradeError> TradeItemInternal(const TradeContext& context, 
            std::size_t itemIndex) const;

        InventoryService& m_inventoryService;
        CharacterService& m_characterService;
        ItemService& m_itemService;
    };
}