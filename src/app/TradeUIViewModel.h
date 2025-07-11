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
        struct Context
        {
            lang::ReactiveProperty<std::string> PlayerCharacterName;
            lang::ReactiveProperty<std::string> TraderCharacterName;

            lang::ReactiveProperty<std::string> PlayerPortraitTextureId;
            lang::ReactiveProperty<std::string> TraderPortraitTextureId;

            lang::ReactiveProperty<std::uint32_t> PlayerCurrentMoney = 0u;
            lang::ReactiveProperty<std::uint32_t> TraderCurrentMoney = 0u;

            lang::ReactiveProperty<float> PlayerCurrentWeight = 0.f;
            lang::ReactiveProperty<float> PlayerMaxWeight = 0.f;

            lang::ReactiveProperty<std::vector<app_domain::InventoryItemDetails>> PlayerItems = {};
            lang::ReactiveProperty<std::vector<app_domain::InventoryItemDetails>> TraderItems = {};

            lang::ReactiveProperty<app_domain::ItemCategory> ItemFilterCategory = app_domain::ItemCategory::All;
            lang::ReactiveProperty<app_domain::ItemSortMode> ItemSortMode = app_domain::ItemSortMode::Latest;
        };

        using OnTradeError = std::function<void(app_domain::TradeError)>;
        using OnTransferPanelConfirm = std::function<void(std::uint32_t count)>;
        using ShowTransferPanelCommand = std::function<void(const app_domain::InventoryItemDetails& item, OnTransferPanelConfirm onConfirm)>;

        TradeUIViewModel(AppContext& appContext);

        Context& GetContext();

        lang::Expected<void, app_domain::TradeError> 
            BeginTrade(const std::string& playerCharacterId, const std::string& traderCharacterId);

        void TradeItem(bool isBuying, std::size_t itemIndex);
        void StackItem(std::size_t fromItemIndex, std::int32_t signedToItemIndex);

        lang::Optional<app_domain::InventoryItemDetails> GetPlayerItem(size_t itemIndex) const;
        lang::Optional<app_domain::InventoryItemDetails> GetTraderItem(size_t itemIndex) const;
        
        void SetShowTransferPanelCommand(ShowTransferPanelCommand command);
        void SetOnTradeError(OnTradeError callback);

    private:
        void UpdateItems();

        uint32_t GetInventoryCurrentMoney(const std::string& inventoryId) const;
        float GetInventoryCurrentWeight(const std::string& inventoryId) const;
        lang::Optional<app_domain::InventoryItemDetails>
            GetInventoryItem(const std::string& inventoryId, size_t itemIndex) const;
        std::vector<app_domain::InventoryItemDetails> 
            LoadInventoryItems(const std::string& inventoryId) const;

        void ShowTransferPanelOrApply(const app_domain::InventoryItemDetails& item,
            std::function<void(std::uint32_t)> onConfirm);
        void InvokeOnTradeError(const app_domain::TradeError& error) const;

        lang::Expected<void, app_domain::TradeError>
            CanTradeItem(bool isBuying, std::size_t itemIndex, std::uint32_t count = app_domain::TradeService::TradeAll) const;
        void TryTradeItem(bool isBuying, std::size_t itemIndex, std::uint32_t count = app_domain::TradeService::TradeAll);
        lang::Expected<void, app_domain::TradeError>
            CanStackItem(std::size_t fromItemIndex, std::size_t toItemIndex, std::uint32_t count = app_domain::InventoryService::TransferAll);
        void TryStackItem(std::size_t fromItemIndex, std::size_t toItemIndex, std::uint32_t count = app_domain::InventoryService::TransferAll);

    private:
        app_domain::ItemService& m_itemService;
        app_domain::CharacterService& m_characterService;
        app_domain::InventoryService& m_inventoryService;
        app_domain::TradeService& m_tradeService;

        Context m_context;

        std::string m_playerCharacterId;
        std::string m_traderCharacterId;
        std::string m_playerInventoryId;
        std::string m_traderInventoryId;

        ShowTransferPanelCommand m_showTransferPanelCommand;
        OnTradeError m_onTradeError;
    };
}