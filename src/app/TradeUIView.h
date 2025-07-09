#pragma once

#include "../core/UIView.h"
#include "../core/StackPanel.h"
#include "../core/CanvasPanel.h"
#include "../core/GridPanel.h"
#include "../app_domain/ItemCategory.h"
#include "../app_domain/ItemSortMode.h"
#include "TradeUIViewModel.h"
#include "ItemGridPanel.h"
#include "ItemTransferPanel.h"
#include "ItemDragSystem.h"

namespace core 
{ 
    class AssetManager; 
    class Image; 
    class TextLabel; 
}

namespace app_domain 
{ 
    class InventoryItemDetails;
}

namespace app
{
    class AppContext; 
    class TooltipPanel;
    class ItemGridPanel;
    class ItemPanel;
    class ItemTransferPanel;
    class ErrorPanel;

    class TradeUIView : public core::UIView
    {
    public:
        TradeUIView(AppContext& appContext, TradeUIViewModel& viewModel);

        void Initialize() override;
        bool HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow) override;
        void Render(sf::RenderWindow& renderWindow) override;
        
    private:
        class CharacterInfoPanel;
        class ItemFilterPanel;
        class ItemSortPanel;

        void BindViewModel();
        void BindItemGridPanel(ItemGridPanel* gridPanel, bool isBuying);

        void SetOnTradeButtonClick(std::function<void()> callback);

        void SetPlayerPortraitTexture(const std::string& textureId);
        void SetPlayerName(const std::string& name);
        void SetPlayerMoney(uint32_t money);
        void SetPlayerWeight(float currentWeight, float maxWeight);

        void SetTraderPortraitTexture(const std::string& textureId);
        void SetTraderName(const std::string& name);
        void SetTraderMoney(uint32_t money);
        void SetTraderWeight(float currentWeight, float maxWeight);

        void SetPlayerItems(const std::vector<app_domain::InventoryItemDetails>& items);
        void SetTraderItems(const std::vector<app_domain::InventoryItemDetails>& items);

        void ShowItemPanel(const app_domain::Item& item, const sf::Vector2f& position) const;
        void HideItemPanel() const;
        void ShowErrorPanel(app_domain::TradeError error) const;

        AppContext& m_appContext;

        TradeUIViewModel& m_viewModel;

        ItemDragSystem m_itemDragSystem;

        core::WidgetId m_playerCharacterInfoPanelId;
        core::WidgetId m_traderCharacterInfoPanelId;
        core::WidgetId m_playerItemGridId;
        core::WidgetId m_traderItemGridId;
        core::WidgetId m_itemFilterPanelId;
        core::WidgetId m_itemSortPanelId;
        core::WidgetId m_itemPanelId;
        core::WidgetId m_itemTransferPanelId;
        core::WidgetId m_errorPanelId;

        std::function<void()> m_onTradeButtonClick;

    private:
        class CharacterInfoPanel final : public core::CanvasPanel
        {
        public:
            CharacterInfoPanel(core::AssetManager& assetManager, bool alignRight);

            void SetCharacterName(const std::string& name);
            void SetPortraitTexture(const std::string& textureId);
            void SetMoney(uint32_t money);
            void SetWeight(float currentWeight, float maxWeight);

        private:
            core::AssetManager& m_assetManager;

            core::TextLabel* m_nameTextLabel = nullptr;
            core::Image* m_portraitImage = nullptr;
            core::TextLabel* m_moneyTextLabel = nullptr;
            core::TextLabel* m_weightTextLabel = nullptr;
        };

        class ItemFilterPanel final : public core::StackPanel
        {
        public:
            struct ItemFilterDescriptor
            {
                app_domain::ItemCategory ItemCategory;
                std::string Name;
                std::string TextureId;

                ItemFilterDescriptor(app_domain::ItemCategory itemCategory, std::string name,
                    std::string textureId);
            };

            using OnFilterButtonClick = std::function<void(app_domain::ItemCategory)>;

            ItemFilterPanel(core::AssetManager& assetManager,
                const std::vector<ItemFilterDescriptor>& itemFilterDescriptors);

            void SetTooltipPanel(TooltipPanel* tooltipPanel);

            void SetOnFilterButtonClick(OnFilterButtonClick callback);

        private:
            TooltipPanel* m_tooltipPanel = nullptr;

            std::vector<core::Button*> m_filterButtons;

            ItemFilterPanel::OnFilterButtonClick m_onFilterButtonClick;
        };

        class ItemSortPanel final : public core::CanvasPanel
        {
        public:
            using ItemSortDescriptor = std::pair<app_domain::ItemSortMode, std::string>;
            using OnSortButtonClick = std::function<void(app_domain::ItemSortMode)>;

            ItemSortPanel(core::AssetManager& assetManager,
                const std::vector<ItemSortDescriptor>& itemSortDescriptors);

            void SetTooltipPanel(TooltipPanel* tooltipPanel);

            void SetOnSortButtonClick(OnSortButtonClick callback);

        private:
            void ToggleItemSortButtonsPanel();

            core::Widget* m_itemSortButtonsPanel = nullptr;
            TooltipPanel* m_tooltipPanel = nullptr;

            ItemSortPanel::OnSortButtonClick m_onSortButtonClick;
        };
    };
}
