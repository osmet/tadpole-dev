#include "Precompiled.h"
#include "TradeUIView.h"

#include "AppContext.h"
#include "../core/CanvasPanel.h"
#include "../core/Image.h"
#include "../core/TextLabel.h"

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

        sf::Color textLabelColor(191u, 163u, 143u, 255u);

        m_mainWidget = std::make_unique<core::CanvasPanel>();
        m_mainWidget->SetSize(renderWindowSize);
        {
            auto* tradeTextLabel = m_mainWidget->CreateWidget<core::TextLabel>();
            tradeTextLabel->SetAnchor(0.5f, 0.f);
            tradeTextLabel->SetPivot(0.5f, 0.f);
            tradeTextLabel->SetLocalPosition(0.f, 12.5f);
            tradeTextLabel->SetFont(regularFont);
            tradeTextLabel->SetFontSize(28u);
            tradeTextLabel->SetText("Trade");
            tradeTextLabel->SetColor(textLabelColor);

            {
                auto* portraitImage = m_mainWidget->CreateWidget<core::Image>();
                portraitImage->SetAnchor(0.5f, 0.f);
                portraitImage->SetPivot(1.f, 0.f);
                portraitImage->SetLocalPosition(-250.f, 75.f);
                portraitImage->SetSize(75.f, 105.f);
                portraitImage->SetTexture(assetManager.GetTexture("UI_Portrait_Alice"));

                auto* nameTextLabel = m_mainWidget->CreateWidget<core::TextLabel>();
                nameTextLabel->SetAnchor(0.5f, 0.f);
                nameTextLabel->SetPivot(0.f, 0.f);
                nameTextLabel->SetLocalPosition(-440.f, 180.f);
                nameTextLabel->SetFont(regularFont);
                nameTextLabel->SetFontSize(20u);
                nameTextLabel->SetText("Player");
            }

            {
                auto* portraitImage = m_mainWidget->CreateWidget<core::Image>();
                portraitImage->SetAnchor(0.5f, 0.f);
                portraitImage->SetPivot(0.f, 0.f);
                portraitImage->SetLocalPosition(250.f, 75.f);
                portraitImage->SetSize(75.f, 105.f);
                portraitImage->SetTexture(assetManager.GetTexture("UI_Portrait_Gale"));

                auto* nameTextLabel = m_mainWidget->CreateWidget<core::TextLabel>();
                nameTextLabel->SetAnchor(0.5f, 0.f);
                nameTextLabel->SetPivot(0.f, 0.f);
                nameTextLabel->SetLocalPosition(40.f, 180.f);
                nameTextLabel->SetFont(regularFont);
                nameTextLabel->SetFontSize(20u);
                nameTextLabel->SetText("Trader");
            }
        }
    }
}