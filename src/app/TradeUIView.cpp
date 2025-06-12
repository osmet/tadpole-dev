#include "Precompiled.h"
#include "TradeUIView.h"

#include "AppContext.h"
#include "../core/CanvasPanel.h"
#include "../core/StackPanel.h"
#include "../core/Image.h"
#include "../core/Button.h"
#include "../core/TextLabel.h"
#include "../app_domain/ItemCategory.h"


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

                float buttonSize = 35.f;
                float spacing = 10.f;
                std::size_t buttonCount = itemFilters.size();
                float totalWidth = buttonCount* buttonSize + (buttonCount - 1) * spacing;
                float offsetX = -totalWidth / 2.0f;

                itemFilterPanel->SetLocalPosition(offsetX, 135.f);
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
                    filterButton->SetSize(buttonSize, buttonSize);
                    filterButton->SetTexture(filterButtonTexture);
                    filterButton->SetNormalColor(filterButtonNormalColor);
                    filterButton->SetHoveredColor(filterButtonHoveredColor);
                    filterButton->SetPressedColor(filterButtonPressedColor);
                    filterButton->SetOnClick([onFilterButtonClick, itemCategory]() { onFilterButtonClick(itemCategory); });

                    auto* itemCategoryImage = filterButton->CreateWidget<core::Image>();
                    itemCategoryImage->SetSize(25.f, 25.f);
                    itemCategoryImage->SetTexture(itemCategoryTexture);

                    auto* filterButtonFrameImage = filterButton->CreateWidget<core::Image>();
                    filterButtonFrameImage->SetSize(buttonSize, buttonSize);
                    filterButtonFrameImage->SetTexture(filterButtonFrameTexture);
                    filterButtonFrameImage->SetColor(filterButtonFrameColor);
                }
            }

            auto& moneyImageTexture = assetManager.GetTexture("UI_Icon_Coin");
            sf::Color moneyImageColor(tradeTextColor);

            auto& weightImageTexture = assetManager.GetTexture("UI_Icon_Weight");

            float characterTextY = 193.f;

            {
                auto* portraitImage = m_mainWidget->CreateWidget<core::Image>();
                portraitImage->SetAnchor(.5f, 0.f);
                portraitImage->SetPivot(1.f, 0.f);
                portraitImage->SetLocalPosition(-250.f, 75.f);
                portraitImage->SetSize(75.f, 105.f);
                portraitImage->SetTexture(assetManager.GetTexture("UI_Portrait_Alice"));

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
                weightImage->SetSize(15.f, 15.f);
                weightImage->SetTexture(weightImageTexture);
                weightImage->SetColor(sf::Color::White);
            }

            {
                auto* portraitImage = m_mainWidget->CreateWidget<core::Image>();
                portraitImage->SetAnchor(.5f, 0.f);
                portraitImage->SetPivot(0.f, 0.f);
                portraitImage->SetLocalPosition(250.f, 75.f);
                portraitImage->SetSize(75.f, 105.f);
                portraitImage->SetTexture(assetManager.GetTexture("UI_Portrait_Gale"));

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

            {

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