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
#include "TooltipPanel.h"

namespace core 
{ 
    class AssetManager; 
}

namespace app_domain 
{ 
    class InventoryItemDetails;
}

namespace app
{
    class AppContext; 

    class TradeUIView final : public core::UIView
    {
    public:
        TradeUIView(AppContext& appContext, TradeUIViewModel& viewModel);

        void Initialize() override;
        bool HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow) override;
        void Render(sf::RenderWindow& renderWindow) override;
        
    private:
        void BindViewModel();
        void BindItemGridPanel(ItemGridPanel* gridPanel, bool isBuying);

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

            core::WidgetId m_nameTextLabelId;
            core::WidgetId m_portraitImageId;
            core::WidgetId m_moneyTextLabelId;
            core::WidgetId m_weightTextLabelId;
        };

        class ItemFilterPanel final : public core::StackPanel, public ITooltipPanelClient
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

            TooltipPanelCommands& GetTooltipPanelCommands() override;

            void SetOnFilterButtonClick(OnFilterButtonClick callback);

        private:
            std::vector<core::WidgetId> m_filterButtonIds;

            TooltipPanelCommands m_tooltipPanelCommands;

            ItemFilterPanel::OnFilterButtonClick m_onFilterButtonClick;
        };

        class ItemSortPanel final : public core::CanvasPanel, public ITooltipPanelClient
        {
        public:
            using ItemSortDescriptor = std::pair<app_domain::ItemSortMode, std::string>;
            using OnSortButtonClick = std::function<void(app_domain::ItemSortMode)>;

            ItemSortPanel(core::AssetManager& assetManager,
                const std::vector<ItemSortDescriptor>& itemSortDescriptors);

            TooltipPanelCommands& GetTooltipPanelCommands() override;

            void SetOnSortButtonClick(OnSortButtonClick callback);

        private:
            void ToggleItemSortButtonsPanel();

            core::WidgetId m_sortByButtonTextLabelId;
            core::WidgetId m_itemSortButtonsPanelId;

            TooltipPanelCommands m_tooltipPanelCommands;

            ItemSortPanel::OnSortButtonClick m_onSortButtonClick;
        };
    };
}
