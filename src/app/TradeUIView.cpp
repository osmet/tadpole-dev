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
#include "ItemSortMode.h"

namespace app
{
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
            sf::Color tradeTextColor(191u, 163u, 143u, 255u);

            {
                auto* tradeTextLabel = m_mainWidget->CreateWidget<core::TextLabel>();
                tradeTextLabel->SetAnchor(.5f, 0.f);
                tradeTextLabel->SetPivot(.5f, 0.f);
                tradeTextLabel->SetLocalPosition(0.f, 24.f);
                tradeTextLabel->SetFont(regularFont);
                tradeTextLabel->SetFontSize(28u);
                tradeTextLabel->SetText("Trade");
                tradeTextLabel->SetColor(tradeTextColor);
            }

            {
                std::vector<std::pair<app_domain::ItemCategory, sf::Texture&>> itemFilters = {
                    {app_domain::ItemCategory::All, assetManager.GetTexture("UI_Icon_Filter_All")},
                    {app_domain::ItemCategory::Equipment, assetManager.GetTexture("UI_Icon_Filter_Equipment")},
                    {app_domain::ItemCategory::BooksAndKeys, assetManager.GetTexture("UI_Icon_Filter_BooksAndKeys")},
                    {app_domain::ItemCategory::Consumables, assetManager.GetTexture("UI_Icon_Filter_Consumables")},
                    {app_domain::ItemCategory::Misc, assetManager.GetTexture("UI_Icon_Filter_Misc")},
                };

                auto onFilterButtonClick = [&](app_domain::ItemCategory itemCategory) { };

                auto* itemFilterPanel = m_mainWidget->CreateWidget<core::StackPanel>();
                itemFilterPanel->SetAnchor(.5f, 0.f);
                itemFilterPanel->SetPivot(.5f, 0.f);

                float buttonSize = 35.f;
                float spacing = 10.f;
                std::size_t buttonCount = itemFilters.size();

                itemFilterPanel->SetLocalPosition(0.f, 135.f);
                itemFilterPanel->SetSpacing(spacing);
                itemFilterPanel->Reserve(buttonCount);
         
                auto& filterButtonTexture = assetManager.GetTexture("UI_Panel_Circle");
                sf::Color filterButtonNormalColor(0u, 0u, 0u, 50u);
                sf::Color filterButtonHoveredColor(128u, 128u, 128u, 50u);
                sf::Color filterButtonPressedColor(255u, 255u, 255u, 50u);
                sf::Color filterButtonFrameColor(128u, 107u, 95u, 255u);
                auto& filterButtonFrameTexture = assetManager.GetTexture("UI_Panel_Circle_Frame");

                for (const auto& itemFilter : itemFilters)
                {
                    auto itemCategory = itemFilter.first;
                    auto& itemCategoryTexture = itemFilter.second;

                    auto* filterButton = itemFilterPanel->CreateWidget<core::Button>();
                    filterButton->SetAnchor(0.f, 0.f);
                    filterButton->SetPivot(0.f, .5f);
                    filterButton->SetSize(buttonSize);
                    filterButton->SetTexture(filterButtonTexture);
                    filterButton->SetNormalColor(filterButtonNormalColor);
                    filterButton->SetHoveredColor(filterButtonHoveredColor);
                    filterButton->SetPressedColor(filterButtonPressedColor);
                    filterButton->SetOnClick([onFilterButtonClick, itemCategory]() { onFilterButtonClick(itemCategory); });

                    auto* itemCategoryImage = filterButton->CreateWidget<core::Image>();
                    itemCategoryImage->SetSize(25.f);
                    itemCategoryImage->SetTexture(itemCategoryTexture);

                    auto* filterButtonFrameImage = filterButton->CreateWidget<core::Image>();
                    filterButtonFrameImage->SetSize(buttonSize);
                    filterButtonFrameImage->SetTexture(filterButtonFrameTexture);
                    filterButtonFrameImage->SetColor(filterButtonFrameColor);
                }
            }

            auto& moneyImageTexture = assetManager.GetTexture("UI_Icon_Coin");
            sf::Color moneyImageColor(tradeTextColor);

            auto& weightImageTexture = assetManager.GetTexture("UI_Icon_Weight");

            float characterTextY = 193.f;

            sf::Vector2f portraitSize(75.f, 105.f);
            sf::Color portraitFrameImageColor(153u, 117u, 92u, 255u);
            float portraitFrameOutlineThickness = 3.f;

            {
                auto* portraitImage = m_mainWidget->CreateWidget<core::Image>();
                portraitImage->SetAnchor(.5f, 0.f);
                portraitImage->SetPivot(1.f, 0.f);
                portraitImage->SetLocalPosition(-250.f, 75.f);
                portraitImage->SetSize(portraitSize);
                portraitImage->SetTexture(assetManager.GetTexture("UI_Portrait_Alice"));

                auto* portraitFrameImage = m_mainWidget->CreateWidget<core::Image>();
                portraitFrameImage->SetAnchor(portraitImage->GetAnchor());
                portraitFrameImage->SetPivot(portraitImage->GetPivot());
                portraitFrameImage->SetLocalPosition(portraitImage->GetLocalPosition());
                portraitFrameImage->SetSize(portraitSize.x - portraitFrameOutlineThickness, 
                    portraitSize.y - portraitFrameOutlineThickness);
                portraitFrameImage->SetColor(sf::Color::Transparent);
                portraitFrameImage->SetOutlineThickness(portraitFrameOutlineThickness);
                portraitFrameImage->SetOutlineColor(portraitFrameImageColor);

                auto* nameTextLabel = m_mainWidget->CreateWidget<core::TextLabel>();
                nameTextLabel->SetAnchor(.5f, 0.f);
                nameTextLabel->SetPivot(0.f, .0f);
                nameTextLabel->SetLocalPosition(-440.f, characterTextY);
                nameTextLabel->SetFont(regularFont);
                nameTextLabel->SetFontSize(20u);
                nameTextLabel->SetText("Alice");

                auto* moneyTextLabel = m_mainWidget->CreateWidget<core::TextLabel>();
                moneyTextLabel->SetAnchor(.5f, 0.f);
                moneyTextLabel->SetPivot(1.f, .0f);
                moneyTextLabel->SetLocalPosition(-250.f - 20.f, characterTextY);
                moneyTextLabel->SetFont(regularFont);
                moneyTextLabel->SetFontSize(20u);
                moneyTextLabel->SetText("1,530");

                auto* moneyImage = m_mainWidget->CreateWidget<core::Image>();
                moneyImage->SetAnchor(.5f, 0.f);
                moneyImage->SetPivot(1.f, .0f);
                moneyImage->SetLocalPosition(-250.f, characterTextY);
                moneyImage->SetSize(12.f, 14.f);
                moneyImage->SetTexture(moneyImageTexture);
                moneyImage->SetColor(moneyImageColor);

                auto* weightTextLabel = m_mainWidget->CreateWidget<core::TextLabel>();
                weightTextLabel->SetAnchor(.5f, 0.f);
                weightTextLabel->SetPivot(1.f, .0f);
                weightTextLabel->SetLocalPosition(-40.f - 22.f, characterTextY);
                weightTextLabel->SetFont(regularFont);
                weightTextLabel->SetFontSize(20u);
                weightTextLabel->SetText("50.5/150");

                auto* weightImage = m_mainWidget->CreateWidget<core::Image>();
                weightImage->SetAnchor(.5f, 0.f);
                weightImage->SetPivot(1.f, .0f);
                weightImage->SetLocalPosition(-40.f, characterTextY - 2.f);
                weightImage->SetSize(15.f);
                weightImage->SetTexture(weightImageTexture);
                weightImage->SetColor(sf::Color::White);
            }

            {
                auto* portraitImage = m_mainWidget->CreateWidget<core::Image>();
                portraitImage->SetAnchor(.5f, 0.f);
                portraitImage->SetPivot(0.f, 0.f);
                portraitImage->SetLocalPosition(250.f, 75.f);
                portraitImage->SetSize(portraitSize);
                portraitImage->SetTexture(assetManager.GetTexture("UI_Portrait_Gale"));

                auto* portraitFrameImage = m_mainWidget->CreateWidget<core::Image>();
                portraitFrameImage->SetAnchor(portraitImage->GetAnchor());
                portraitFrameImage->SetPivot(portraitImage->GetPivot());
                portraitFrameImage->SetLocalPosition(portraitImage->GetLocalPosition());
                portraitFrameImage->SetSize(portraitSize.x - portraitFrameOutlineThickness,
                    portraitSize.y - portraitFrameOutlineThickness);
                portraitFrameImage->SetColor(sf::Color::Transparent);
                portraitFrameImage->SetOutlineThickness(portraitFrameOutlineThickness);
                portraitFrameImage->SetOutlineColor(portraitFrameImageColor);


                auto* nameTextLabel = m_mainWidget->CreateWidget<core::TextLabel>();
                nameTextLabel->SetAnchor(.5f, 0.f);
                nameTextLabel->SetPivot(0.f, .0f);
                nameTextLabel->SetLocalPosition(40.f, characterTextY);
                nameTextLabel->SetFont(regularFont);
                nameTextLabel->SetFontSize(20u);
                nameTextLabel->SetText("Gale");

                auto* moneyTextLabel = m_mainWidget->CreateWidget<core::TextLabel>();
                moneyTextLabel->SetAnchor(.5f, 0.f);
                moneyTextLabel->SetPivot(1.f, .0f);
                moneyTextLabel->SetLocalPosition(325.f - 20.f, characterTextY);
                moneyTextLabel->SetFont(regularFont);
                moneyTextLabel->SetFontSize(20u);
                moneyTextLabel->SetText("350");

                auto* moneyImage = m_mainWidget->CreateWidget<core::Image>();
                moneyImage->SetAnchor(.5f, 0.f);
                moneyImage->SetPivot(1.f, .0f);
                moneyImage->SetLocalPosition(325.f, characterTextY);
                moneyImage->SetSize(12.f, 14.f);
                moneyImage->SetTexture(moneyImageTexture);
                moneyImage->SetColor(moneyImageColor);
            }

            size_t itemGridColumnCount = 7;
            size_t itemGridRowCount = 12;
            size_t itemGridCellCount = itemGridColumnCount * itemGridRowCount;

            auto& itemSlotRarityGlowTexture = assetManager.GetTexture("UI_Overlay_ItemSlot_RarityGlow");
            auto& itemTestImage = assetManager.GetTexture("UI_Item_VeryRareWizardRobe");

            float cellSize = 54.f;
            float spacing = 4.f;
            float outlineThickness = 2.f;
            float iconSize = cellSize - outlineThickness * 2.f;

            {
                auto* itemGrid = m_mainWidget->CreateWidget<core::GridPanel>();
                itemGrid->SetAnchor(0.5f, 0.f);
                itemGrid->SetPivot(1.f, 0.f);
                itemGrid->SetLocalPosition(-40.f, 215.f);
                itemGrid->SetColumnCount(itemGridColumnCount);
                itemGrid->SetCellSize(cellSize);
                itemGrid->SetSpacing(spacing);

                for (size_t index = 0; index < itemGridCellCount; ++index)
                {
                    auto* itemSlot = itemGrid->CreateWidget<core::Button>();
                    itemSlot->SetAnchor(0.f, 0.f);
                    itemSlot->SetPivot(0.f, 0.f);
                    itemSlot->SetColor(sf::Color::Transparent);
               
                    auto* rarityGlowImage = itemSlot->CreateWidget<core::Image>();
                    rarityGlowImage->SetSize(iconSize);
                    rarityGlowImage->SetTexture(itemSlotRarityGlowTexture);
                    rarityGlowImage->SetColor(sf::Color::Green);
                    rarityGlowImage->SetActive(false);

                    auto* iconImage = itemSlot->CreateWidget<core::Image>();
                    iconImage->SetSize(iconSize);
                    iconImage->SetTexture(itemTestImage);
                    iconImage->SetActive(false);

                    auto* frameImage = itemSlot->CreateWidget<core::Image>();
                    frameImage->SetSize(iconSize);
                    frameImage->SetColor(sf::Color::Transparent);
                    frameImage->SetOutlineThickness(outlineThickness);
                    frameImage->SetOutlineColor(sf::Color(64u, 54u, 48u, 255u));

                    auto* countTextLabel = itemSlot->CreateWidget<core::TextLabel>();
                    countTextLabel->SetAnchor(1.f, 1.f);
                    countTextLabel->SetPivot(1.f, 1.f);
                    countTextLabel->SetLocalPosition(-4.f, -8.f);
                    countTextLabel->SetFont(regularFont);
                    countTextLabel->SetFontSize(20.f);
                    countTextLabel->SetText("3");
                    countTextLabel->SetOutlineThickness(1.f);
                    countTextLabel->SetActive(false);

                    auto* selectImage = itemSlot->CreateWidget<core::Image>();
                    selectImage->SetSize(iconSize + 2.f);
                    selectImage->SetColor(sf::Color::Transparent);
                    selectImage->SetOutlineThickness(outlineThickness);
                    selectImage->SetOutlineColor(sf::Color::White);
                    selectImage->SetActive(false);

                    if (index < 4)
                    {
                        rarityGlowImage->SetActive(true);
                        iconImage->SetActive(true);

                        if (index == 3)
                        {
                            countTextLabel->SetActive(true);
                        }

                        if (index < 3)
                        {
                            selectImage->SetActive(true);
                            selectImage->SetOutlineColor(sf::Color(255u, 255u, 255u, 200u));
                        }

                        if (index < 1)
                        {
                            selectImage->SetActive(true);
                            selectImage->SetOutlineColor(sf::Color::White);

                            countTextLabel->SetActive(true);
                        }
                    }
                }
            }

            {
                auto* itemGrid = m_mainWidget->CreateWidget<core::GridPanel>();
                itemGrid->SetAnchor(0.5f, 0.f);
                itemGrid->SetPivot(0.f, 0.f);
                itemGrid->SetLocalPosition(40.f, 215.f);
                itemGrid->SetColumnCount(itemGridColumnCount);
                itemGrid->SetCellSize(54.f);
                itemGrid->SetSpacing(4.f);

                for (size_t index = 0; index < itemGridCellCount; ++index)
                {
                    auto* itemSlot = itemGrid->CreateWidget<core::Button>();
                    itemSlot->SetAnchor(0.f, 0.f);
                    itemSlot->SetPivot(0.f, 0.f);
                    itemSlot->SetColor(sf::Color::Transparent);

                    auto* rarityGlowImage = itemSlot->CreateWidget<core::Image>();
                    rarityGlowImage->SetSize(iconSize);
                    rarityGlowImage->SetTexture(itemSlotRarityGlowTexture);
                    rarityGlowImage->SetColor(sf::Color::Green);
                    rarityGlowImage->SetActive(false);

                    auto* iconImage = itemSlot->CreateWidget<core::Image>();
                    iconImage->SetSize(iconSize);
                    iconImage->SetTexture(itemTestImage);
                    iconImage->SetActive(false);

                    auto* frameImage = itemSlot->CreateWidget<core::Image>();
                    frameImage->SetSize(iconSize);
                    frameImage->SetColor(sf::Color::Transparent);
                    frameImage->SetOutlineThickness(outlineThickness);
                    frameImage->SetOutlineColor(sf::Color(64u, 54u, 48u, 255u));

                    auto* selectImage = itemSlot->CreateWidget<core::Image>();
                    selectImage->SetSize(iconSize + 2.f);
                    selectImage->SetColor(sf::Color::Transparent);
                    selectImage->SetOutlineThickness(outlineThickness);
                    selectImage->SetOutlineColor(sf::Color::White);
                    selectImage->SetActive(false);

                    if (index < 4)
                    {
                        rarityGlowImage->SetActive(true);
                        iconImage->SetActive(true);

                        if (index < 3)
                        {
                            selectImage->SetActive(true);
                            selectImage->SetOutlineColor(sf::Color(255u, 255u, 255u, 200u));
                        }

                        if (index < 1)
                        {
                            selectImage->SetActive(true);
                            selectImage->SetOutlineColor(sf::Color::White);
                        }
                    }
                }
            }

            {
                std::vector<std::pair<app::ItemSortMode, std::string>> sortModes = {
                    {app::ItemSortMode::Type, "Type"},
                    {app::ItemSortMode::Value, "Value"},
                    {app::ItemSortMode::Weight, "Weight"},
                    {app::ItemSortMode::Name, "Name"}
                };

                auto onSortButtonClick = [&](ItemSortMode mode) {};

                auto* sortByButton = m_mainWidget->CreateWidget<core::Button>();
                sortByButton->SetAnchor(.5f, 1.f);
                sortByButton->SetPivot(0.f, 1.f);
                sortByButton->SetLocalPosition(-650.f, -95.f);
                sortByButton->SetSize(185.f, 30.f);
                sortByButton->SetColor(sf::Color::Transparent);

                const sf::Texture& sortIconTexture = assetManager.GetTexture("UI_Icon_Sort");

                auto* sortByButtonIconImage = sortByButton->CreateWidget<core::Image>();
                sortByButtonIconImage->SetAnchor(0.f, .5f);
                sortByButtonIconImage->SetPivot(0.f, .5f);
                sortByButtonIconImage->SetLocalPosition(8.f, 0.f);
                sortByButtonIconImage->SetSize(16.f);
                sortByButtonIconImage->SetTexture(sortIconTexture);

                auto* sortByButtonTextLabel = sortByButton->CreateWidget<core::TextLabel>();
                sortByButtonTextLabel->SetAnchor(0.f, .5f);
                sortByButtonTextLabel->SetPivot(0.f, .5f);
                sortByButtonTextLabel->SetLocalPosition(30.f, -1.f);
                sortByButtonTextLabel->SetFont(regularFont);
                sortByButtonTextLabel->SetFontSize(18u);
                sortByButtonTextLabel->SetText("Sort By");

                sf::Color sortByFrameColor(tradeTextColor);
                sortByFrameColor.a = 128u;

                auto* sortByButtonFrame = sortByButton->CreateWidget<core::Image>();
                sortByButtonFrame->SetSize(185.f, 26.f);
                sortByButtonFrame->SetColor(sf::Color::Transparent);
                sortByButtonFrame->SetOutlineThickness(2.f);
                sortByButtonFrame->SetOutlineColor(sortByFrameColor);

                sf::Vector2f itemSortPanelSize(185.f, 165.f);
                sf::Vector2f itemSortPanelPosition(sortByButton->GetLocalPosition());
                itemSortPanelPosition.y -= 40.f;

                auto* itemSortPanelFrameImage = m_mainWidget->CreateWidget<core::Image>();
                itemSortPanelFrameImage->SetAnchor(.5f, 1.f);
                itemSortPanelFrameImage->SetPivot(0.f, 1.f);
                itemSortPanelFrameImage->SetLocalPosition(itemSortPanelPosition);
                itemSortPanelFrameImage->SetSize(itemSortPanelSize);
                itemSortPanelFrameImage->SetColor(sf::Color(0u, 0u, 0u, 50u));
                itemSortPanelFrameImage->SetOutlineThickness(2.f);
                itemSortPanelFrameImage->SetOutlineColor(sortByFrameColor);

                itemSortPanelPosition.y += 10.f;

                auto* itemSortPanel = m_mainWidget->CreateWidget<core::StackPanel>();
                itemSortPanel->SetAnchor(.5f, 1.f);
                itemSortPanel->SetPivot(0.f, 1.f);
                itemSortPanel->SetLocalPosition(itemSortPanelPosition);
                itemSortPanel->SetSize(itemSortPanelSize);
                itemSortPanel->Reserve(sortModes.size());
                itemSortPanel->SetOrientation(core::StackPanel::Orientation::Vertical);
                itemSortPanel->SetSpacing(0.f);
                itemSortPanel->SetSizeToContent(false);

                for (const auto& sortMode : sortModes)
                {
                    auto sortModeValue = sortMode.first;
                    const auto& sortModeName = sortMode.second;

                    auto* sortButton = itemSortPanel->CreateWidget<core::Button>();
                    sortButton->SetAnchor(0.f, 0.f);
                    sortButton->SetPivot(0.f, 0.f);
                    sortButton->SetSize(185.f, 35.f);
                    sortButton->SetColor(sf::Color::Transparent);

                    auto* sortButtonTextLabel = sortButton->CreateWidget<core::TextLabel>();
                    sortButtonTextLabel->SetAnchor(0.f, .5f);
                    sortButtonTextLabel->SetPivot(0.f, .5f);
                    sortButtonTextLabel->SetLocalPosition(18.f, 0.f);
                    sortButtonTextLabel->SetFont(regularFont);
                    sortButtonTextLabel->SetFontSize(18u);
                    sortButtonTextLabel->SetText(sortModeName);
                }
            }

            {
                auto* tradeButton = m_mainWidget->CreateWidget<core::Button>();
                tradeButton->SetAnchor(0.5f, 1.f);
                tradeButton->SetPivot(0.5f, 1.f);
                tradeButton->SetLocalPosition(0.f, -35.f);
                tradeButton->SetSize(250.f, 40.f);
                tradeButton->SetColor(sf::Color(48, 58, 64));

                auto* tradeButtonText = tradeButton->CreateWidget<core::TextLabel>();
                tradeButtonText->SetLocalPosition(0.f, 0.f);
                tradeButtonText->SetFont(regularFont);
                tradeButtonText->SetFontSize(18);
                tradeButtonText->SetText("TRADE");
                tradeButtonText->SetColor(sf::Color::White);
            }
        }
    }
}