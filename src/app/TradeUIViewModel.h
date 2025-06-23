#pragma once

#include "AppContext.h"
#include "../app_domain/TradeService.h"
#include "../app_domain/CharacterService.h"
#include "../app_domain/InventoryService.h"
#include "../app_domain/ItemService.h"

namespace app
{
    class TradeUIViewModel
    {
    public:
        using OnTradeBegin = std::function<void()>;
        using OnPlayerInventoryUpdate = std::function<void()>;
        using OnTraderInventoryUpdate = std::function<void()>;

        TradeUIViewModel(AppContext& appContext);

        tl::expected<void, app_domain::TradeError> BeginTrade(
            const std::string& playerCharacterId,
            const std::string& traderCharacterId);

        void SetOnTradeBegin(OnTradeBegin callback);
        void SetOnPlayerInventoryUpdate(OnPlayerInventoryUpdate callback);
        void SetOnTraderInventoryUpdate(OnTraderInventoryUpdate callback);

        tl::expected<void, app_domain::TradeError> CanBuyItem(std::size_t itemIndex) const;
        tl::expected<void, app_domain::TradeError> CanSellItem(std::size_t itemIndex) const;

        tl::expected<void, app_domain::TradeError> BuyItem(std::size_t itemIndex);
        tl::expected<void, app_domain::TradeError> SellItem(std::size_t itemIndex);

        const std::string& GetPlayerName() const;
        const std::string& GetTraderName() const;

        const std::string& GetPlayerPortraitTextureId() const;
        const std::string& GetTraderPortraitTextureId() const;

        uint32_t GetPlayerMoney() const;
        uint32_t GetTraderMoney() const;

        float GetPlayerCurrentWeight() const;
        float GetPlayerMaxWeight() const;

        const app_domain::Item* GetPlayerItemByIndex(size_t index) const;
        const app_domain::Item* GetTraderItemByIndex(size_t index) const;
        
        std::vector<const app_domain::Item*> GetPlayerItems() const;
        std::vector<const app_domain::Item*> GetTraderItems() const;

    private:
        void InvokeOnInventoryUpdates();
        uint32_t GetInventoryMoney(const std::string& inventoryId) const;
        const app_domain::Item* GetInventoryItemByIndex(const std::string& inventoryId, size_t index) const;
        std::vector<const app_domain::Item*> GetInventoryItems(const std::string& inventoryId) const;

    private:
        app_domain::ItemService& m_itemService;
        app_domain::CharacterService& m_characterService;
        app_domain::InventoryService& m_inventoryService;
        app_domain::TradeService& m_tradeService;

        std::optional<std::string> m_playerCharacterId;
        std::optional<std::string> m_traderCharacterId;
        std::optional<std::string> m_playerInventoryId;
        std::optional<std::string> m_traderInventoryId;

        std::string m_playerCharacterName;
        std::string m_traderCharacterName;
        std::string m_playerPortrait;
        std::string m_traderPortrait;
        float m_playerMaxWeight = 0.0f;

        OnTradeBegin m_onTradeBegin;
        OnPlayerInventoryUpdate m_onPlayerInventoryUpdate;
        OnTraderInventoryUpdate m_onTraderInventoryUpdate;
    };
}