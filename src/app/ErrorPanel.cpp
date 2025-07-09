#include "Precompiled.h"
#include "ErrorPanel.h"

#include "../core/AssetManager.h"
#include "../core/Image.h"
#include "../core/TextLabel.h"
#include "../core/Button.h"
#include "ScreenOverlayPanel.h"

namespace app
{
    ErrorPanel::ErrorPanel(core::AssetManager& assetManager, const sf::Vector2f& renderWindowSize)
    {
        SetAnchor(0.5f, 0.5f);
        SetPivot(0.5f, 0.5f);
        SetActive(false);

        auto* screenOverlayImage = CreateWidget<ScreenOverlayPanel>(renderWindowSize);

        auto& regularFont = assetManager.GetFont("Mignon_Regular");

        auto* backgroundImage = CreateWidget<core::Image>();
        backgroundImage->SetColor(sf::Color(0u, 0u, 0u, 150u));
        backgroundImage->SetOutlineThickness(2.f);
        backgroundImage->SetOutlineColor(sf::Color(153u, 117u, 92u, 255u));
        backgroundImage->SetSize(540.f, 140.f);
        backgroundImage->SetLocalPosition(0.f, -16.f);

        auto* titleTextLabel = backgroundImage->CreateWidget<core::TextLabel>();
        titleTextLabel->SetAnchor(0.5f, 0.f);
        titleTextLabel->SetPivot(0.5f, 0.f);
        titleTextLabel->SetLocalPosition(0.f, 36.f);
        titleTextLabel->SetFont(regularFont);
        titleTextLabel->SetFontSize(28u);
        titleTextLabel->SetText("Not enough gold");
        m_titleTextLabelId = titleTextLabel->GetId();

        auto* descriptionTextLabel = backgroundImage->CreateWidget<core::TextLabel>();
        descriptionTextLabel->SetAnchor(0.5f, 0.f);
        descriptionTextLabel->SetPivot(0.5f, 0.f);
        descriptionTextLabel->SetLocalPosition(0.f, 76.f);
        descriptionTextLabel->SetFont(regularFont);
        descriptionTextLabel->SetFontSize(18u);
        descriptionTextLabel->SetColor(sf::Color(222u, 214u, 203u, 255u));
        descriptionTextLabel->SetText("Not enough gold to complete this deal.");
        m_descriptionTextLabelId = descriptionTextLabel->GetId();

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
        {
            auto textLabel = confirmButton->CreateWidget<core::TextLabel>();
            textLabel->SetFont(regularFont);
            textLabel->SetFontSize(18u);
            textLabel->SetText("OK");
        }
    }

    void ErrorPanel::Show(const std::string& title, const std::string& description)
    {
        if (auto* titleTextLabel = FindWidgetById<core::TextLabel>(m_titleTextLabelId))
            titleTextLabel->SetText(title);

        if (auto* descriptionTextLabel = FindWidgetById<core::TextLabel>(m_descriptionTextLabelId))
            descriptionTextLabel->SetText(description);

        SetActive(true);
    }

    void ErrorPanel::SetOnConfirm(OnConfirm callback)
    {
        m_onConfirm = std::move(callback);
    }
}