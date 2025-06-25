#include "Precompiled.h"
#include "TradeUIView.h"

#include "AppContext.h"
#include "../core/CanvasPanel.h"
#include "../core/StackPanel.h"
#include "../core/Image.h"
#include "../core/Button.h"
#include "../core/TextLabel.h"
#include "../core/GridPanel.h"
#include "ItemGridPanel.h"
#include "ItemPanel.h"
#include "TradeUIViewModel.h"

namespace app
{
    TradeUIView::TooltipPanel::TooltipPanel(core::AssetManager& assetManager)
    {
        SetAnchor(0.f, 0.f);
        SetPivot(0.5f, 0.5f);
        SetActive(false);

        auto& regularFont = assetManager.GetFont("Mignon_Regular");

        auto* frameImage = CreateWidget<core::Image>();
        frameImage->SetColor(sf::Color(0u, 0u, 0u, 150u));
        frameImage->SetOutlineThickness(2.f);
        frameImage->SetOutlineColor(sf::Color(191u, 163u, 143u, 128u));
        m_frameImage = frameImage;

        auto* textLabel = frameImage->CreateWidget<core::TextLabel>();
        textLabel->SetLocalPosition(0.f, -1.f);
        textLabel->SetFont(regularFont);
        textLabel->SetFontSize(18u);
        m_textLabel = textLabel;
    }

    void TradeUIView::TooltipPanel::Show(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& offset)
    {
        if (!m_textLabel || !m_frameImage)
            return;

        sf::Vector2f padding(20.f, 14.f);
        sf::Vector2f positionWithOffset(position.x + offset.x, position.y + offset.y);

        bool isAbove = offset.y <= 0.f;

        SetPivot(0.f, isAbove ? 1.f : 0.f);
        SetLocalPosition(positionWithOffset);
        SetActive(true);

        m_textLabel->SetText(text);

        sf::Vector2f frameSize(m_textLabel->GetSize());
        frameSize.x += 2.f * padding.x;
        frameSize.y += 2.f * padding.y;

        m_frameImage->SetSize(frameSize);
        m_frameImage->SetAnchor(GetPivot());
        m_frameImage->SetPivot(GetPivot());
    }

    void TradeUIView::TooltipPanel::Hide()
    {
        SetActive(false);
    }

    TradeUIView::ErrorPanel::ErrorPanel(core::AssetManager& assetManager, const sf::Vector2f& renderWindowSize)
    {
        SetAnchor(0.f, 0.f);
        SetPivot(0.5f, 0.5f);
        SetActive(false);

        auto& regularFont = assetManager.GetFont("Mignon_Regular");

        auto* overlayImage = CreateWidget<core::Image>();
        overlayImage->SetAnchor(.5f, .5f);
        overlayImage->SetPivot(.5f, .5f);
        overlayImage->SetSize(renderWindowSize);
        overlayImage->SetLocalPosition(renderWindowSize.x / 2.f, renderWindowSize.y / 2.f);
        overlayImage->SetColor(sf::Color(0u, 0u, 0u, 150u));
        
        auto* backgroundImage = CreateWidget<core::Image>();
        backgroundImage->SetColor(sf::Color(0u, 0u, 0u, 150u));
        backgroundImage->SetOutlineThickness(2.f);
        backgroundImage->SetOutlineColor(sf::Color(153u, 117u, 92u, 255u));
        backgroundImage->SetSize(540.f, 140.f);
        backgroundImage->SetLocalPosition(renderWindowSize.x / 2.f, renderWindowSize.y / 2.f - 16.f);

        auto* titleTextLabel = backgroundImage->CreateWidget<core::TextLabel>();
        titleTextLabel->SetAnchor(0.5f, 0.f);
        titleTextLabel->SetPivot(0.5f, 0.f);
        titleTextLabel->SetLocalPosition(0.f, 36.f);
        titleTextLabel->SetFont(regularFont);
        titleTextLabel->SetFontSize(28u);
        titleTextLabel->SetText("Not enough gold");
        m_titleTextLabel = titleTextLabel;

        auto* descriptionTextLabel = backgroundImage->CreateWidget<core::TextLabel>();
        descriptionTextLabel->SetAnchor(0.5f, 0.f);
        descriptionTextLabel->SetPivot(0.5f, 0.f);
        descriptionTextLabel->SetLocalPosition(0.f, 76.f);
        descriptionTextLabel->SetFont(regularFont);
        descriptionTextLabel->SetFontSize(18u);
        descriptionTextLabel->SetColor(sf::Color(222u, 214u, 203u, 255u));
        descriptionTextLabel->SetText("Not enough gold to complete this deal.");
        m_descriptionTextLabel = descriptionTextLabel;

        auto* confirmButton = backgroundImage->CreateWidget<core::Button>();
        confirmButton->SetAnchor(0.5f, 1.f);
        confirmButton->SetPivot(0.5f, 0.5f);
        confirmButton->SetLocalPosition(0.f, 0.f);
        confirmButton->SetSize(160.f, 34.f);
        confirmButton->SetColor(sf::Color(48u, 58u, 64u, 255u));
        confirmButton->SetOnClick([this]() 
        { 
            SetActive(false); 

            if (m_onConfirm)
                m_onConfirm();
        });

        auto confirmText = confirmButton->CreateWidget<core::TextLabel>();
        confirmText->SetFont(regularFont);
        confirmText->SetFontSize(18u);
        confirmText->SetText("OK");
    }

    void TradeUIView::ErrorPanel::Show(const std::string& title, const std::string& description, OnConfirm callback)
    {
        if (m_titleTextLabel)
            m_titleTextLabel->SetText(title);

        if (m_descriptionTextLabel)
            m_descriptionTextLabel->SetText(description);

        m_onConfirm = std::move(callback);

        SetActive(true);
    }

    TradeUIView::ItemFilterPanel::ItemFilterDescriptor::ItemFilterDescriptor(app_domain::ItemCategory itemCategory, std::string name,
        std::string textureId)
        : ItemCategory(itemCategory), Name(std::move(name)), TextureId(std::move(textureId))
    {
    }

    TradeUIView::ItemFilterPanel::ItemFilterPanel(core::AssetManager& assetManager,
        const std::vector<ItemFilterDescriptor>& itemFilterDescriptors)
    {
        std::size_t buttonCount = itemFilterDescriptors.size();

        if (buttonCount == 0u)
        {
            SetActive(false);
            return;
        }

        float buttonSize = 35.f;
        float spacing = 10.f;

        SetSpacing(spacing);
        Reserve(buttonCount);

        auto& filterButtonTexture = assetManager.GetTexture("UI_Panel_Circle");
        sf::Color filterButtonNormalColor(0u, 0u, 0u, 50u);
        sf::Color filterButtonHoveredColor(128u, 128u, 128u, 50u);
        sf::Color filterButtonPressedColor(255u, 255u, 255u, 50u);
        sf::Color filterButtonDisabledColor(255u, 255u, 255u, 50u);
        sf::Color filterButtonFrameColor(128u, 107u, 95u, 255u);
        auto& filterButtonFrameTexture = assetManager.GetTexture("UI_Panel_Circle_Frame");


        for (const auto& itemFilterDescriptor : itemFilterDescriptors)
        {
            auto itemCategory = itemFilterDescriptor.ItemCategory;
            auto& name = itemFilterDescriptor.Name;
            auto& textureId = itemFilterDescriptor.TextureId;

            auto& texture = assetManager.GetTexture(textureId);

            auto* filterButton = CreateWidget<core::Button>();
            filterButton->SetAnchor(0.f, 0.f);
            filterButton->SetPivot(0.f, .5f);
            filterButton->SetSize(buttonSize);
            filterButton->SetTexture(filterButtonTexture);
            filterButton->SetNormalColor(filterButtonNormalColor);
            filterButton->SetHoveredColor(filterButtonHoveredColor);
            filterButton->SetPressedColor(filterButtonPressedColor);
            filterButton->SetDisabledColor(filterButtonDisabledColor);
            filterButton->SetOnClick([this, filterButton, itemCategory]()
            { 
                for (auto* button : m_filterButtons)
                {
                    if (button)
                        button->SetInteractable(true);
                }

                if (filterButton)
                    filterButton->SetInteractable(false);

                if (m_onFilterButtonClick) 
                    m_onFilterButtonClick(itemCategory); 

            });
            filterButton->SetOnHoverIn([this, filterButton, name](const sf::Vector2f& mousePosition) 
            {
                if (m_tooltipPanel && filterButton) 
                    m_tooltipPanel->Show(name, filterButton->GetPosition(), sf::Vector2f(6.f, -12.f));
            });
            filterButton->SetOnHoverOut([this] 
            {
                if (m_tooltipPanel)
                    m_tooltipPanel->Hide();
            });
            m_filterButtons.push_back(filterButton);

            auto* itemCategoryImage = filterButton->CreateWidget<core::Image>();
            itemCategoryImage->SetSize(25.f);
            itemCategoryImage->SetTexture(texture);

            auto* filterButtonFrameImage = filterButton->CreateWidget<core::Image>();
            filterButtonFrameImage->SetSize(buttonSize);
            filterButtonFrameImage->SetTexture(filterButtonFrameTexture);
            filterButtonFrameImage->SetColor(filterButtonFrameColor);
        }

        if (m_filterButtons[0])
            m_filterButtons[0]->SetInteractable(false);
    }

    void TradeUIView::ItemFilterPanel::SetTooltipPanel(TooltipPanel* tooltipPanel)
    {
        m_tooltipPanel = tooltipPanel;
    }

    void TradeUIView::ItemFilterPanel::SetOnFilterButtonClick(OnFilterButtonClick callback)
    {
        m_onFilterButtonClick = std::move(callback);
    }

    TradeUIView::ItemSortPanel::ItemSortPanel(core::AssetManager& assetManager,
        const std::vector<ItemSortDescriptor>& itemSortDescriptors)
    {
        std::size_t buttonCount = itemSortDescriptors.size();
        if (buttonCount == 0u)
        {
            SetActive(false);
            return;
        }

        float buttonSize = 35.f;
        float spacing = 10.f;
        auto& regularFont = assetManager.GetFont("Mignon_Regular");
        auto& sortIconTexture = assetManager.GetTexture("UI_Icon_Sort");

        auto* sortByButton = CreateWidget<core::Button>();
        sortByButton->SetAnchor(.5f, 1.f);
        sortByButton->SetPivot(0.f, 1.f);
        sortByButton->SetSize(185.f, 30.f);
        sortByButton->SetColor(sf::Color::Transparent);
        sortByButton->SetOnClick([this] { ToggleItemSortButtonsPanel(); });
        sortByButton->SetOnHoverIn([this, sortByButton](const sf::Vector2f& mousePosition) 
        {
            if (m_tooltipPanel && sortByButton)
                m_tooltipPanel->Show("Sort By", sf::Vector2f(mousePosition.x, sortByButton->GetPosition().y), sf::Vector2f(6.f, 24.f + sortByButton->GetSize().y));
        });
        sortByButton->SetOnHoverOut([this] 
        { 
            if (m_tooltipPanel)
                m_tooltipPanel->Hide();
        });

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
        sortByButtonTextLabel->SetText(itemSortDescriptors[0].second);

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
        itemSortPanelImage->SetActive(false);
        m_itemSortButtonsPanel = itemSortPanelImage;

        auto* itemSortPanelFrameImage = itemSortPanelImage->CreateWidget<core::Image>();
        itemSortPanelFrameImage->SetSize(itemSortPanelSize);
        itemSortPanelFrameImage->SetTexture(itemSortPanelFrameTexture);
        itemSortPanelFrameImage->SetColor(sortByFrameColor);

        auto* itemSortButtonsPanel = itemSortPanelImage->CreateWidget<core::StackPanel>();
        itemSortButtonsPanel->SetAnchor(0.f, 0.f);
        itemSortButtonsPanel->SetPivot(0.f, 0.f);
        itemSortButtonsPanel->SetLocalPosition(0.f, 10.f);
        itemSortButtonsPanel->Reserve(itemSortDescriptors.size());
        itemSortButtonsPanel->SetOrientation(core::StackPanel::Orientation::Vertical);
        itemSortButtonsPanel->SetSpacing(0.f);
        itemSortButtonsPanel->SetSizeToContent(false);

        for (const auto& itemSortDescriptor : itemSortDescriptors)
        {
            auto sortMode = itemSortDescriptor.first;
            const auto& sortModeName = itemSortDescriptor.second;

            auto* sortButton = itemSortButtonsPanel->CreateWidget<core::Button>();
            sortButton->SetAnchor(0.f, 0.f);
            sortButton->SetPivot(0.f, 0.f);
            sortButton->SetSize(185.f, 35.f);
            sortButton->SetColor(sf::Color::Transparent);
            sortButton->SetOnClick([this, sortMode, sortModeName, sortByButtonTextLabel]()
            { 
                if (sortByButtonTextLabel)
                    sortByButtonTextLabel->SetText(sortModeName);

                ToggleItemSortButtonsPanel();

                if (m_onSortButtonClick) 
                    m_onSortButtonClick(sortMode); 
            });

            auto* sortButtonTextLabel = sortButton->CreateWidget<core::TextLabel>();
            sortButtonTextLabel->SetAnchor(0.f, .5f);
            sortButtonTextLabel->SetPivot(0.f, .5f);
            sortButtonTextLabel->SetLocalPosition(20.f, 0.f);
            sortButtonTextLabel->SetFont(regularFont);
            sortButtonTextLabel->SetFontSize(18u);
            sortButtonTextLabel->SetText(sortModeName);
        }
    }

    void TradeUIView::ItemSortPanel::SetTooltipPanel(TooltipPanel* tooltipPanel)
    {
        m_tooltipPanel = tooltipPanel;
    }

    void TradeUIView::ItemSortPanel::SetOnSortButtonClick(OnSortButtonClick callback)
    {
        m_onSortButtonClick = std::move(callback);
    }

    void TradeUIView::ItemSortPanel::ToggleItemSortButtonsPanel()
    {
        if (m_itemSortButtonsPanel)
            m_itemSortButtonsPanel->SetActive(!m_itemSortButtonsPanel->IsActiveSelf());
    }

    TradeUIView::CharacterInfoPanel::CharacterInfoPanel(core::AssetManager& assetManager, bool alignRight)
        : m_assetManager(assetManager)
    {
        auto& font = assetManager.GetFont("Mignon_Regular");
        auto& portraitFrameTexture = assetManager.GetTexture("UI_Panel_Portrait_Frame");
        auto& moneyIconTexture = assetManager.GetTexture("UI_Icon_Coin");
        auto& weightIconTexture = assetManager.GetTexture("UI_Icon_Weight");

        sf::Color portraitFrameColor(153u, 117u, 92u, 255u);
        float portraitFrameOutlineThickness = 3.f;
        sf::Vector2f portraitSize(72.f, 105.f);
        float portraitPositionX = alignRight ? 250.f : -250.f;
        float namePositionX = alignRight ? 40.f : -440.f;
        float moneyPositionX = alignRight ? portraitPositionX + portraitSize.x - 20.f : portraitPositionX - 20.f;
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
        moneyIconImage->SetColor(sf::Color(191u, 163u, 143u, 255u));
        moneyIconImage->SetTexture(moneyIconTexture);

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
            weightIconImage->SetTexture(weightIconTexture);
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
            m_weightTextLabel->SetColor(currentWeight > maxWeight ? sf::Color::Red : sf::Color::White);

            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%.1f/%d", currentWeight, (std::uint32_t)maxWeight);

            m_weightTextLabel->SetText(buffer);
        }
    }

    TradeUIView::TradeUIView(AppContext& appContext, TradeUIViewModel& viewModel)
        : m_appContext(appContext)
        , m_viewModel(viewModel)
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
                    { app_domain::ItemCategory::All, "All", "UI_Icon_Filter_All" },
                    { app_domain::ItemCategory::Equipment, "Equipment", "UI_Icon_Filter_Equipment" },
                    { app_domain::ItemCategory::BooksAndKeys, "Books and Keys", "UI_Icon_Filter_BooksAndKeys" },
                    { app_domain::ItemCategory::Consumables, "Consumables", "UI_Icon_Filter_Consumables" },
                    { app_domain::ItemCategory::ScrollsAndTools, "Scrolls and Tools", "UI_Icon_Filter_ScrollsAndTools" },
                    { app_domain::ItemCategory::Misc, "Miscellaneous", "UI_Icon_Filter_Misc" },
                };

                auto* itemFilterPanel = m_mainWidget->CreateWidget<ItemFilterPanel>(assetManager, itemFilterDescriptors);
                itemFilterPanel->SetAnchor(.5f, 0.f);
                itemFilterPanel->SetPivot(.5f, 0.f);
                itemFilterPanel->SetLocalPosition(0.f, 135.f);
                m_itemFilterPanel = itemFilterPanel;
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
                  { app_domain::ItemSortMode::Latest, "Latest" },
                  { app_domain::ItemSortMode::Type, "Type" },
                  { app_domain::ItemSortMode::Value, "Value" },
                  { app_domain::ItemSortMode::Weight, "Weight" },
                };

                auto* itemSortPanel = m_mainWidget->CreateWidget<ItemSortPanel>(assetManager, itemSortDescriptors);
                itemSortPanel->SetAnchor(.5f, 1.f);
                itemSortPanel->SetPivot(0.f, 1.f);
                itemSortPanel->SetLocalPosition(-492.f, -95.f);
                m_itemSortPanel = itemSortPanel;
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
                tradeButton->SetOnClick([onTradeButtonClick = m_onTradeButtonClick]() { if (onTradeButtonClick) onTradeButtonClick(); });

                auto* tradeButtonText = tradeButton->CreateWidget<core::TextLabel>();
                tradeButtonText->SetLocalPosition(0.f, 0.f);
                tradeButtonText->SetFont(regularFont);
                tradeButtonText->SetFontSize(18);
                tradeButtonText->SetText("TRADE");
                tradeButtonText->SetColor(sf::Color::White);
            }

            {
                auto* itemPanel = m_mainWidget->CreateWidget<ItemPanel>(assetManager);
                m_itemPanel = itemPanel;
            }

            {
                auto* tooltipPanel = m_mainWidget->CreateWidget<TooltipPanel>(assetManager);

                if (m_itemFilterPanel) 
                    m_itemFilterPanel->SetTooltipPanel(tooltipPanel);

                if (m_itemSortPanel) 
                    m_itemSortPanel->SetTooltipPanel(tooltipPanel);
            }

            {
                auto* errorPanel = m_mainWidget->CreateWidget<ErrorPanel>(assetManager, renderWindowSize);
                m_errorPanel = errorPanel;
            }
        }

        BindViewModel();
    }

    void TradeUIView::HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow)
    {
        if (m_errorPanel && m_errorPanel->IsActiveSelf())
        {
            m_errorPanel->HandleEvent(event, renderWindow);
            return;
        }
            
        UIView::HandleEvent(event, renderWindow);
    }
    
    void TradeUIView::BindViewModel() {
        m_viewModel.SetOnTradeBegin([this]() 
        {
            SetPlayerName(m_viewModel.GetPlayerName());
            SetPlayerPortraitTexture(m_viewModel.GetPlayerPortraitTextureId());

            SetTraderName(m_viewModel.GetTraderName());
            SetTraderPortraitTexture(m_viewModel.GetTraderPortraitTextureId());
        });

        m_viewModel.SetOnPlayerItemsUpdate([this]() 
        {
            SetPlayerItems(m_viewModel.GetPlayerItems());

            SetPlayerMoney(m_viewModel.GetPlayerMoney());

            SetPlayerWeight(m_viewModel.GetPlayerCurrentWeight(), m_viewModel.GetPlayerMaxWeight());
        });

        m_viewModel.SetOnTraderItemsUpdate([this]() 
        {
            SetTraderItems(m_viewModel.GetTraderItems());

            SetTraderMoney(m_viewModel.GetTraderMoney());
        });

        if (m_playerItemGrid)
        {
            m_playerItemGrid->SetOnItemSlotClick([this](std::size_t index) 
            {
                auto canTradeResult = m_viewModel.CanSellItem(index);
                if (!canTradeResult.has_value())
                {
                    ShowErrorPanel(canTradeResult.error());
                    return;
                }

                auto tradeResult = m_viewModel.SellItem(index);
                if (!tradeResult.has_value())
                    ShowErrorPanel(tradeResult.error());
            });

            m_playerItemGrid->SetOnItemSlotHoverIn([this](std::size_t index, const sf::Vector2f& position) 
            {
                auto item = m_viewModel.GetPlayerItem(index);
                if (item)
                    ShowItemPanel(item->GetItem(), position);
            });

            m_playerItemGrid->SetOnItemSlotHoverOut([this](std::size_t index) 
            {
                HideItemPanel();
            });
        }

        if (m_traderItemGrid)
        {
            m_traderItemGrid->SetOnItemSlotClick([this](std::size_t index) 
            {
                auto canTradeResult = m_viewModel.CanBuyItem(index);
                if (!canTradeResult.has_value())
                {
                    ShowErrorPanel(canTradeResult.error());
                    return;
                }

                auto tradeResult = m_viewModel.BuyItem(index);
                if (!tradeResult.has_value())
                    ShowErrorPanel(tradeResult.error());
            });

            m_traderItemGrid->SetOnItemSlotHoverIn([this](std::size_t index, const sf::Vector2f& position) 
            {
                auto item = m_viewModel.GetTraderItem(index);
                if (item)
                    ShowItemPanel(item->GetItem(), position);
            });

            m_traderItemGrid->SetOnItemSlotHoverOut([this](std::size_t index) 
            {
                HideItemPanel();
            });
        }

        if (m_itemFilterPanel)
        {
            m_itemFilterPanel->SetOnFilterButtonClick([this](app_domain::ItemCategory itemCategory)
            {
                m_viewModel.SetItemFilterCategory(itemCategory);
            });
        }
            
        if (m_itemSortPanel)
        {
            m_itemSortPanel->SetOnSortButtonClick([this](app_domain::ItemSortMode itemSortMode)
            {
                m_viewModel.SetItemSortMode(itemSortMode);
            });
        }
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
            m_playerCharacterInfoPanel->SetCharacterName(name);
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
            m_traderCharacterInfoPanel->SetCharacterName(name);
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

    void TradeUIView::SetPlayerItems(const std::vector<app_domain::InventoryItemDetails>& items)
    {
        if (m_playerItemGrid)
            m_playerItemGrid->SetItems(items);
    }

    void TradeUIView::SetTraderItems(const std::vector<app_domain::InventoryItemDetails>& items)
    {
        if (m_traderItemGrid)
            m_traderItemGrid->SetItems(items);
    }

    void TradeUIView::ShowItemPanel(const app_domain::Item& item, const sf::Vector2f& position)
    {
        if (!m_itemPanel)
            return;

        m_itemPanel->Show(item, position, sf::Vector2f(60.f, 60.f), m_appContext.GetRenderWindowSize().y);
    }

    void TradeUIView::HideItemPanel()
    {
        if (!m_itemPanel)
            return;

        m_itemPanel->Hide();
    }

    void TradeUIView::ShowErrorPanel(app_domain::TradeError error)
    {
        if (!m_errorPanel)
            return;

        using app_domain::TradeError;

        switch (error)
        {
        case TradeError::NotEnoughMoney:
            m_errorPanel->Show(
                "Insufficient Gold",
                "A character has insufficient gold to complete this transaction.",
                []() {}
            );
            break;
        default:
            break;
        }
    }
}