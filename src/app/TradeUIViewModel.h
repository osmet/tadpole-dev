#pragma once

#include "AppContext.h"
#include "../app_domain/TradeService.h"
#include "../app_domain/CharacterService.h"
#include "../app_domain/InventoryService.h"
#include "../app_domain/ItemService.h"
#include "../app_domain/ItemSortMode.h"

namespace app
{
    class TradeUIViewModel
    {
    public:
        using OnTradeBegin = std::function<void()>;
        using OnItemsUpdate = std::function<void()>;
        using OnFilterCategoryChange = std::function<void(app_domain::ItemCategory)>;
        using OnSortModeChange = std::function<void(app_domain::ItemSortMode)>;

        TradeUIViewModel(AppContext& appContext);

        tl::expected<void, app_domain::TradeError> BeginTrade(
            const std::string& playerCharacterId,
            const std::string& traderCharacterId);

        tl::expected<void, app_domain::TradeError> 
            CanBuyItem(std::size_t itemIndex, std::uint32_t count = app_domain::TradeService::TradeAll) const;
        tl::expected<void, app_domain::TradeError> 
            CanSellItem(std::size_t itemIndex, std::uint32_t count = app_domain::TradeService::TradeAll) const;

        tl::expected<void, app_domain::TradeError> 
            BuyItem(std::size_t itemIndex, std::uint32_t count = app_domain::TradeService::TradeAll);
        tl::expected<void, app_domain::TradeError> 
            SellItem(std::size_t itemIndex, std::uint32_t count = app_domain::TradeService::TradeAll);

        const std::string& GetPlayerName() const;
        const std::string& GetTraderName() const;

        const std::string& GetPlayerPortraitTextureId() const;
        const std::string& GetTraderPortraitTextureId() const;

        uint32_t GetPlayerMoney() const;
        uint32_t GetTraderMoney() const;

        float GetPlayerCurrentWeight() const;
        float GetPlayerMaxWeight() const;

        const std::optional<app_domain::InventoryItemDetails> GetPlayerItem(size_t itemIndex) const;
        const std::optional<app_domain::InventoryItemDetails> GetTraderItem(size_t itemIndex) const;
        
        const std::vector<app_domain::InventoryItemDetails>& GetPlayerItems() const;
        const std::vector<app_domain::InventoryItemDetails>& GetTraderItems() const;

        void SetItemFilterCategory(app_domain::ItemCategory itemFilterCategory);
        void SetItemSortMode(app_domain::ItemSortMode itemSortMode);

        void SetOnTradeBegin(OnTradeBegin callback);
        void SetOnPlayerItemsUpdate(OnItemsUpdate callback);
        void SetOnTraderItemsUpdate(OnItemsUpdate callback);

    private:
        void UpdateItems();
        uint32_t GetInventoryMoney(const std::string& inventoryId) const;
        std::optional<app_domain::InventoryItemDetails> GetInventoryItem(const std::string& inventoryId, size_t itemIndex) const;
        void LoadInventoryItems(const std::string& inventoryId, std::vector<app_domain::InventoryItemDetails>& out_items) const;

    private:
        app_domain::ItemService& m_itemService;
        app_domain::CharacterService& m_characterService;
        app_domain::InventoryService& m_inventoryService;
        app_domain::TradeService& m_tradeService;

        std::string m_playerCharacterId;
        std::string m_traderCharacterId;
        std::string m_playerInventoryId;
        std::string m_traderInventoryId;

        std::string m_playerCharacterName;
        std::string m_traderCharacterName;
        std::string m_playerPortrait;
        std::string m_traderPortrait;
        float m_playerMaxWeight = 0.0f;

        std::vector<app_domain::InventoryItemDetails> m_playerItemsCache;
        std::vector<app_domain::InventoryItemDetails> m_traderItemsCache;

        app_domain::ItemCategory m_itemFilterCategory = app_domain::ItemCategory::All;
        app_domain::ItemSortMode m_itemSortMode = app_domain::ItemSortMode::Latest;

        OnTradeBegin m_onTradeBegin;
        OnItemsUpdate m_onPlayerItemsUpdate;
        OnItemsUpdate m_onTraderItemsUpdate;
    };
}