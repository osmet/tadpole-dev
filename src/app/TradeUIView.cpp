#include "Precompiled.h"
#include "TradeUIView.h"

#include "AppContext.h"
#include "../core/CanvasPanel.h"
#include "../core/StackPanel.h"
#include "../core/Image.h"
#include "../core/Button.h"
#include "../core/TextLabel.h"
#include "../core/GridPanel.h"
#include "../app_domain/ItemCategory.h"
#include "ItemGridPanel.h"

namespace app
{

    TradeUIView::ItemFilterPanel::ItemFilterPanel(core::AssetManager& assetManager,
        const std::vector<ItemFilterDescriptor>& itemFilterDescriptors,
        OnFilterButtonClick& onFilterButtonClick)
    {
        float buttonSize = 35.f;
        float spacing = 10.f;
        std::size_t buttonCount = itemFilterDescriptors.size();

        SetSpacing(spacing);
        Reserve(buttonCount);

        auto& filterButtonTexture = assetManager.GetTexture("UI_Panel_Circle");
        sf::Color filterButtonNormalColor(0u, 0u, 0u, 50u);
        sf::Color filterButtonHoveredColor(128u, 128u, 128u, 50u);
        sf::Color filterButtonPressedColor(255u, 255u, 255u, 50u);
        sf::Color filterButtonFrameColor(128u, 107u, 95u, 255u);
        auto& filterButtonFrameTexture = assetManager.GetTexture("UI_Panel_Circle_Frame");

        for (const auto& itemFilterDescriptor : itemFilterDescriptors)
        {
            auto itemCategory = itemFilterDescriptor.first;
            auto& itemCategoryTexture = itemFilterDescriptor.second;

            auto* filterButton = CreateWidget<core::Button>();
            filterButton->SetAnchor(0.f, 0.f);
            filterButton->SetPivot(0.f, .5f);
            filterButton->SetSize(buttonSize);
            filterButton->SetTexture(filterButtonTexture);
            filterButton->SetNormalColor(filterButtonNormalColor);
            filterButton->SetHoveredColor(filterButtonHoveredColor);
            filterButton->SetPressedColor(filterButtonPressedColor);
            filterButton->SetOnClick([&, itemCategory]() { if (onFilterButtonClick) onFilterButtonClick(itemCategory); });

            auto* itemCategoryImage = filterButton->CreateWidget<core::Image>();
            itemCategoryImage->SetSize(25.f);
            itemCategoryImage->SetTexture(itemCategoryTexture);

            auto* filterButtonFrameImage = filterButton->CreateWidget<core::Image>();
            filterButtonFrameImage->SetSize(buttonSize);
            filterButtonFrameImage->SetTexture(filterButtonFrameTexture);
            filterButtonFrameImage->SetColor(filterButtonFrameColor);
        }
    }

    TradeUIView::ItemSortPanel::ItemSortPanel(core::AssetManager& assetManager,
        const std::vector<ItemSortDescriptor>& itemSortDescriptors,
        OnSortButtonClick& onSortButtonClick)
    {
        float buttonSize = 35.f;
        float spacing = 10.f;
        std::size_t buttonCount = itemSortDescriptors.size();

        auto& regularFont = assetManager.GetFont("Mignon_Regular");
        auto& sortIconTexture = assetManager.GetTexture("UI_Icon_Sort");

        auto* sortByButton = CreateWidget<core::Button>();
        sortByButton->SetAnchor(.5f, 1.f);
        sortByButton->SetPivot(0.f, 1.f);
        sortByButton->SetSize(185.f, 30.f);
        sortByButton->SetColor(sf::Color::Transparent);

        auto* sortByButtonIconImage = sortByButton->CreateWidget<core::Image>();
        sortByButtonIconImage->SetAnchor(0.f, .5f);
        sortByButtonIconImage->SetPivot(0.f, .5f);
        sortByButtonIconImage->SetLocalPosition(10.f, 0.f);
        sortByButtonIconImage->SetSize(16.f);
        sortByButtonIconImage->SetTexture(sortIconTexture);

        auto* sortByButtonTextLabel = sortByButton->CreateWidget<core::TextLabel>();
        sortByButtonTextLabel->SetAnchor(0.f, .5f);
        sortByButtonTextLabel->SetPivot(0.f, .5f);
        sortByButtonTextLabel->SetLocalPosition(33.f, -1.f);
        sortByButtonTextLabel->SetFont(regularFont);
        sortByButtonTextLabel->SetFontSize(18u);
        sortByButtonTextLabel->SetText("Sort By");

        sf::Color sortByFrameColor(sf::Color(191u, 163u, 143u, 128u));
        auto& sortByButtonFrameTexture = assetManager.GetTexture("UI_Panel_SortByButton_Frame");

        auto* sortByButtonFrame = sortByButton->CreateWidget<core::Image>();
        sortByButtonFrame->SetSize(185.f, 30.f);
        sortByButtonFrame->SetTexture(sortByButtonFrameTexture);
        sortByButtonFrame->SetColor(sortByFrameColor);

        sf::Vector2f itemSortPanelSize(185.f, 165.f);
        sf::Vector2f itemSortPanelPosition(0.f, -40.f);
        auto& itemSortPanelTexture = assetManager.GetTexture("UI_Panel_ItemSort");
        auto& itemSortPanelFrameTexture = assetManager.GetTexture("UI_Panel_ItemSort_Frame");

        auto* itemSortPanelImage = CreateWidget<core::Image>();
        itemSortPanelImage->SetAnchor(.5f, 1.f);
        itemSortPanelImage->SetPivot(0.f, 1.f);
        itemSortPanelImage->SetLocalPosition(itemSortPanelPosition);
        itemSortPanelImage->SetSize(itemSortPanelSize);
        itemSortPanelImage->SetTexture(itemSortPanelTexture);
        itemSortPanelImage->SetColor(sf::Color(0u, 0u, 0u, 150u));

        auto* itemSortPanelFrameImage = CreateWidget<core::Image>();
        itemSortPanelFrameImage->SetAnchor(.5f, 1.f);
        itemSortPanelFrameImage->SetPivot(0.f, 1.f);
        itemSortPanelFrameImage->SetLocalPosition(itemSortPanelPosition);
        itemSortPanelFrameImage->SetSize(itemSortPanelSize);
        itemSortPanelFrameImage->SetTexture(itemSortPanelFrameTexture);
        itemSortPanelFrameImage->SetColor(sortByFrameColor);

        itemSortPanelPosition.y += 10.f;

        auto* itemSortPanel = CreateWidget<core::StackPanel>();
        itemSortPanel->SetAnchor(.5f, 1.f);
        itemSortPanel->SetPivot(0.f, 1.f);
        itemSortPanel->SetLocalPosition(itemSortPanelPosition);
        itemSortPanel->SetSize(itemSortPanelSize);
        itemSortPanel->Reserve(itemSortDescriptors.size());
        itemSortPanel->SetOrientation(core::StackPanel::Orientation::Vertical);
        itemSortPanel->SetSpacing(0.f);
        itemSortPanel->SetSizeToContent(false);

        for (const auto& itemSortDescriptor : itemSortDescriptors)
        {
            auto sortMode = itemSortDescriptor.first;
            const auto& sortModeName = itemSortDescriptor.second;

            auto* sortButton = itemSortPanel->CreateWidget<core::Button>();
            sortButton->SetAnchor(0.f, 0.f);
            sortButton->SetPivot(0.f, 0.f);
            sortButton->SetSize(185.f, 35.f);
            sortButton->SetColor(sf::Color::Transparent);
            sortButton->SetOnClick([&, sortMode]() { if (onSortButtonClick) onSortButtonClick(sortMode); });

            auto* sortButtonTextLabel = sortButton->CreateWidget<core::TextLabel>();
            sortButtonTextLabel->SetAnchor(0.f, .5f);
            sortButtonTextLabel->SetPivot(0.f, .5f);
            sortButtonTextLabel->SetLocalPosition(20.f, 0.f);
            sortButtonTextLabel->SetFont(regularFont);
            sortButtonTextLabel->SetFontSize(18u);
            sortButtonTextLabel->SetText(sortModeName);
        }
    }

    TradeUIView::CharacterInfoPanel::CharacterInfoPanel(core::AssetManager& assetManager, bool alignRight)
        : m_assetManager(assetManager)
    {
        auto& font = assetManager.GetFont("Mignon_Regular");
        auto& portraitFrameTexture = assetManager.GetTexture("UI_Panel_Portrait_Frame");
        auto& moneyTexture = assetManager.GetTexture("UI_Icon_Coin");
        auto& weightTexture = assetManager.GetTexture("UI_Icon_Weight");

        sf::Color portraitFrameColor(153u, 117u, 92u, 255u);
        float portraitFrameOutlineThickness = 3.f;
        sf::Vector2f portraitSize(72.f, 105.f);
        float portraitPositionX = alignRight ? 250.f : -250.f;
        float namePositionX = alignRight ? 40.f : -440.f;
        float moneyPositionX = alignRight ? 325.f : -250.f - 20.f;
        float weightPositionX = alignRight ? 40.f : -40.f;
        float valuesPositionY = 193.f;

        SetAnchor(.5f, 0.f);
        SetPivot(.5f, 0.f);

        auto* portraitImage = CreateWidget<core::Image>();
        portraitImage->SetAnchor(.5f, 0.f);
        portraitImage->SetPivot(alignRight ? 0.f : 1.f, 0.f);
        portraitImage->SetLocalPosition(portraitPositionX, 75.f);
        portraitImage->SetSize(portraitSize);
        m_portraitImage = portraitImage;

        auto* portraitFrameImage = portraitImage->CreateWidget<core::Image>();
        portraitFrameImage->SetSize(portraitSize);
        portraitFrameImage->SetTexture(portraitFrameTexture);
        portraitFrameImage->SetColor(portraitFrameColor);

        auto* nameTextLabel = CreateWidget<core::TextLabel>();
        nameTextLabel->SetAnchor(.5f, 0.f);
        nameTextLabel->SetPivot(0.f, 0.f);
        nameTextLabel->SetLocalPosition(namePositionX, valuesPositionY);
        nameTextLabel->SetFont(font);
        nameTextLabel->SetFontSize(20u);
        m_nameTextLabel = nameTextLabel;

        auto* moneyTextLabel = CreateWidget<core::TextLabel>();
        moneyTextLabel->SetAnchor(.5f, 0.f);
        moneyTextLabel->SetPivot(1.f, 0.f);
        moneyTextLabel->SetLocalPosition(moneyPositionX, valuesPositionY);
        moneyTextLabel->SetFont(font);
        moneyTextLabel->SetFontSize(20u);
        m_moneyTextLabel = moneyTextLabel;

        auto* moneyIconImage = CreateWidget<core::Image>();
        moneyIconImage->SetAnchor(.5f, 0.f);
        moneyIconImage->SetPivot(1.f, 0.f);
        moneyIconImage->SetLocalPosition(moneyPositionX + 20.f, valuesPositionY);
        moneyIconImage->SetSize(12.f, 14.f);
        moneyIconImage->SetColor(sf::Color(191, 163, 143));
        moneyIconImage->SetTexture(moneyTexture);

        if (!alignRight)
        {
            auto* weightTextLabel = CreateWidget<core::TextLabel>();
            weightTextLabel->SetAnchor(.5f, 0.f);
            weightTextLabel->SetPivot(1.f, 0.f);
            weightTextLabel->SetLocalPosition(weightPositionX - 22.f, valuesPositionY);
            weightTextLabel->SetFont(font);
            weightTextLabel->SetFontSize(20u);
            m_weightTextLabel = weightTextLabel;

            auto* weightIconImage = CreateWidget<core::Image>();
            weightIconImage->SetAnchor(.5f, 0.f);
            weightIconImage->SetPivot(1.f, 0.f);
            weightIconImage->SetLocalPosition(weightPositionX, valuesPositionY - 2.f);
            weightIconImage->SetSize(15.f);
            weightIconImage->SetTexture(weightTexture);
        }
    }

    void TradeUIView::CharacterInfoPanel::SetCharacterName(const std::string& name)
    {
        if (m_nameTextLabel)
            m_nameTextLabel->SetText(name);
    }

    void TradeUIView::CharacterInfoPanel::SetPortraitTexture(const std::string& textureId)
    {
        if (m_portraitImage)
            m_portraitImage->SetTexture(m_assetManager.GetTexture(textureId));
    }

    void TradeUIView::CharacterInfoPanel::SetMoney(uint32_t money)
    {
        if (m_moneyTextLabel)
            m_moneyTextLabel->SetText(std::to_string(money));
    }

    void TradeUIView::CharacterInfoPanel::SetWeight(float currentWeight, float maxWeight)
    {
        if (m_weightTextLabel)
        {
            char buf[32];
            snprintf(buf, sizeof(buf), "%.1f/%d", currentWeight, (std::uint32_t)maxWeight);

            m_weightTextLabel->SetText(buf);
        }
    }

    TradeUIView::TradeUIView(AppContext& appContext)
        : m_appContext(appContext)
    {
    }

    void TradeUIView::Initialize()
    {
        auto& assetManager = m_appContext.GetAssetManager();

        auto renderWindowSize = m_appContext.GetRenderWindowSize();

        auto& regularFont = assetManager.GetFont("Mignon_Regular");

        m_mainWidget = std::make_unique<core::CanvasPanel>();
        m_mainWidget->SetAnchor(0.f, 0.f);
        m_mainWidget->SetPivot(0.f, 0.f);
        m_mainWidget->SetSize(renderWindowSize);
        {
            {
                auto* tradeTextLabel = m_mainWidget->CreateWidget<core::TextLabel>();
                tradeTextLabel->SetAnchor(.5f, 0.f);
                tradeTextLabel->SetPivot(.5f, 0.f);
                tradeTextLabel->SetLocalPosition(0.f, 24.f);
                tradeTextLabel->SetFont(regularFont);
                tradeTextLabel->SetFontSize(28u);
                tradeTextLabel->SetText("Trade");
                tradeTextLabel->SetColor(sf::Color(191u, 163u, 143u, 255u));
            }

            {
                std::vector<ItemFilterPanel::ItemFilterDescriptor> itemFilterDescriptors = {
                    {app_domain::ItemCategory::All, assetManager.GetTexture("UI_Icon_Filter_All")},
                    {app_domain::ItemCategory::Equipment, assetManager.GetTexture("UI_Icon_Filter_Equipment")},
                    {app_domain::ItemCategory::BooksAndKeys, assetManager.GetTexture("UI_Icon_Filter_BooksAndKeys")},
                    {app_domain::ItemCategory::Consumables, assetManager.GetTexture("UI_Icon_Filter_Consumables")},
                    {app_domain::ItemCategory::ScrollsAndTools, assetManager.GetTexture("UI_Icon_Filter_ScrollsAndTools")},
                    {app_domain::ItemCategory::Misc, assetManager.GetTexture("UI_Icon_Filter_Misc")},
                };

                auto* itemFilterPanel = m_mainWidget->CreateWidget<ItemFilterPanel>(assetManager, itemFilterDescriptors, m_onItemFilterButtonClick);
                itemFilterPanel->SetAnchor(.5f, 0.f);
                itemFilterPanel->SetPivot(.5f, 0.f);
                itemFilterPanel->SetLocalPosition(0.f, 135.f);
            }

            {
                auto* playerCharacterInfoPanel = m_mainWidget->CreateWidget<CharacterInfoPanel>(assetManager, false);
                m_playerCharacterInfoPanel = playerCharacterInfoPanel;

                auto* traderCharacterInfoPanel = m_mainWidget->CreateWidget<CharacterInfoPanel>(assetManager, true);
                m_traderCharacterInfoPanel = traderCharacterInfoPanel;
            }

            {
                size_t columnCount = 7;
                size_t rowCount = 12;

                float cellSize = 54.f;
                float spacing = 4.f;

                auto* playerItemGrid = m_mainWidget->CreateWidget<ItemGridPanel>(assetManager, columnCount, rowCount, cellSize, spacing);
                playerItemGrid->SetAnchor(0.5f, 0.f);
                playerItemGrid->SetPivot(1.f, 0.f);
                playerItemGrid->SetLocalPosition(-40.f, 215.f);
                m_playerItemGrid = playerItemGrid;

                auto* traderItemGrid = m_mainWidget->CreateWidget<ItemGridPanel>(assetManager, columnCount, rowCount, cellSize, spacing);
                traderItemGrid->SetAnchor(0.5f, 0.f);
                traderItemGrid->SetPivot(0.f, 0.f);
                traderItemGrid->SetLocalPosition(40.f, 215.f);
                m_traderItemGrid = traderItemGrid;
            }

            {
                std::vector<ItemSortPanel::ItemSortDescriptor> itemSortDescriptors = {
                  {ItemSortMode::Type, "Type"},
                  {ItemSortMode::Value, "Value"},
                  {ItemSortMode::Weight, "Weight"},
                  {ItemSortMode::Name, "Name"}
                };

                auto* itemSortPanel = m_mainWidget->CreateWidget<ItemSortPanel>(assetManager, itemSortDescriptors, m_onItemSortButtonClick);
                itemSortPanel->SetAnchor(.5f, 1.f);
                itemSortPanel->SetPivot(0.f, 1.f);
                itemSortPanel->SetLocalPosition(-492.f, -95.f);
            }

            {
                auto& tradeButtonTexture = assetManager.GetTexture("UI_Panel_TradeButton");

                auto* tradeButton = m_mainWidget->CreateWidget<core::Button>();
                tradeButton->SetAnchor(0.5f, 1.f);
                tradeButton->SetPivot(0.5f, 1.f);
                tradeButton->SetLocalPosition(0.f, -35.f);
                tradeButton->SetSize(250.f, 40.f);
                tradeButton->SetTexture(tradeButtonTexture);
                tradeButton->SetColor(sf::Color(48, 58, 64));
                tradeButton->SetOnClick([&]() { if (m_onTradeButtonClick) m_onTradeButtonClick(); });

                auto* tradeButtonText = tradeButton->CreateWidget<core::TextLabel>();
                tradeButtonText->SetLocalPosition(0.f, 0.f);
                tradeButtonText->SetFont(regularFont);
                tradeButtonText->SetFontSize(18);
                tradeButtonText->SetText("TRADE");
                tradeButtonText->SetColor(sf::Color::White);
            }
        }
    }

    void TradeUIView::SetOnItemFilterButtonClick(ItemFilterPanel::OnFilterButtonClick callback)
    {
        m_onItemFilterButtonClick = std::move(callback);
    }

    void TradeUIView::SetOnItemSortButtonClick(ItemSortPanel::OnSortButtonClick callback)
    {
        m_onItemSortButtonClick = std::move(callback);
    }

    void TradeUIView::SetOnTradeButtonClick(std::function<void()> callback)
    {
        m_onTradeButtonClick = std::move(callback);
    }

    void TradeUIView::SetPlayerPortraitTexture(const std::string& textureId)
    {
        if (m_playerCharacterInfoPanel)
            m_playerCharacterInfoPanel->SetPortraitTexture(textureId);
    }

    void TradeUIView::SetPlayerName(const std::string& name)
    {
        if (m_playerCharacterInfoPanel)
            m_playerCharacterInfoPanel->SetName(name);
    }

    void TradeUIView::SetPlayerMoney(uint32_t money)
    {
        if (m_playerCharacterInfoPanel)
            m_playerCharacterInfoPanel->SetMoney(money);
    }

    void TradeUIView::SetPlayerWeight(float currentWeight, float maxWeight)
    {
        if (m_playerCharacterInfoPanel)
            m_playerCharacterInfoPanel->SetWeight(currentWeight, maxWeight);
    }

    void TradeUIView::SetTraderPortraitTexture(const std::string& textureId)
    {
        if (m_traderCharacterInfoPanel)
            m_traderCharacterInfoPanel->SetPortraitTexture(textureId);
    }

    void TradeUIView::SetTraderName(const std::string& name)
    {
        if (m_traderCharacterInfoPanel)
            m_traderCharacterInfoPanel->SetName(name);
    }

    void TradeUIView::SetTraderMoney(uint32_t money)
    {
        if (m_traderCharacterInfoPanel)
            m_traderCharacterInfoPanel->SetMoney(money);
    }

    void TradeUIView::SetTraderWeight(float currentWeight, float maxWeight)
    {
        if (m_traderCharacterInfoPanel)
            m_traderCharacterInfoPanel->SetWeight(currentWeight, maxWeight);
    }

    void TradeUIView::SetPlayerItems(const std::vector<const app_domain::Item*>& items)
    {
        if (m_playerItemGrid)
            m_playerItemGrid->SetItems(items);
    }

    void TradeUIView::SetTraderItems(const std::vector<const app_domain::Item*>& items)
    {
        if (m_traderItemGrid)
            m_traderItemGrid->SetItems(items);
    }
}