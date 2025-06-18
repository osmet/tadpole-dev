#pragma once

#include "../core/UIView.h"
#include "../core/StackPanel.h"
#include "../core/CanvasPanel.h"
#include "../core/GridPanel.h"
#include "../app_domain/ItemCategory.h"

namespace core 
{ 
    class AssetManager; 
    class Image; 
    class TextLabel; 
}

namespace app_domain 
{ 
    struct Item; 
}

namespace app
{
    class AppContext; class ItemGridPanel;

    class TradeUIView : public core::UIView
    {
    private:
        class TooltipPanel final : public core::CanvasPanel
        {
        public:
            TooltipPanel(core::AssetManager& assetManager, const sf::Vector2f& margins);

            void Show(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& offset);
            void Hide();
        private:
            const sf::Vector2f m_margins;

            core::Image* m_frameImage = nullptr;
            core::TextLabel* m_textLabel = nullptr;
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
                const std::vector<ItemFilterDescriptor>& itemFilterDescriptors,
                OnFilterButtonClick& onFilterButtonClick);

            void SetTooltipPanel(TooltipPanel* tooltipPanel);

        private:
            TooltipPanel* m_tooltipPanel = nullptr;
        };

        enum class ItemSortMode : uint8_t
        {
            Type = 0,
            Value = 1,
            Weight = 2,
            Name = 3
        };

        class ItemSortPanel final : public core::CanvasPanel
        {
        public:
            using ItemSortDescriptor = std::pair<ItemSortMode, std::string>;
            using OnSortButtonClick = std::function<void(ItemSortMode)>;

            ItemSortPanel(core::AssetManager& assetManager,
                const std::vector<ItemSortDescriptor>& itemSortDescriptors,
                OnSortButtonClick& onSortButtonClick);

            void SetTooltipPanel(TooltipPanel* tooltipPanel);

        private:
            void ToggleItemSortButtonsPanel();

            core::Widget* m_itemSortButtonsPanel = nullptr;
            TooltipPanel* m_tooltipPanel = nullptr;
        };

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

    public:
        TradeUIView(AppContext& appContext);

        void Initialize() override;

        void SetOnItemFilterButtonClick(ItemFilterPanel::OnFilterButtonClick callback);
        void SetOnItemSortButtonClick(ItemSortPanel::OnSortButtonClick callback);
        void SetOnTradeButtonClick(std::function<void()> callback);

        void SetPlayerPortraitTexture(const std::string& textureId);
        void SetPlayerName(const std::string& name);
        void SetPlayerMoney(uint32_t money);
        void SetPlayerWeight(float currentWeight, float maxWeight);

        void SetTraderPortraitTexture(const std::string& textureId);
        void SetTraderName(const std::string& name);
        void SetTraderMoney(uint32_t money);
        void SetTraderWeight(float currentWeight, float maxWeight);

        void SetPlayerItems(const std::vector<const app_domain::Item*>& items);
        void SetTraderItems(const std::vector<const app_domain::Item*>& items);
        
    private:
        AppContext& m_appContext;

        CharacterInfoPanel* m_playerCharacterInfoPanel = nullptr;
        CharacterInfoPanel* m_traderCharacterInfoPanel = nullptr;
        ItemGridPanel* m_playerItemGrid = nullptr;
        ItemGridPanel* m_traderItemGrid = nullptr;
        ItemFilterPanel* m_itemFilterPanel = nullptr;
        ItemSortPanel* m_itemSortPanel = nullptr;

        ItemFilterPanel::OnFilterButtonClick m_onItemFilterButtonClick;
        ItemSortPanel::OnSortButtonClick m_onItemSortButtonClick;
        std::function<void()> m_onTradeButtonClick;
    };
}
