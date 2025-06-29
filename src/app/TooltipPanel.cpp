#include "Precompiled.h"
#include "TooltipPanel.h"

#include "../core/AssetManager.h"
#include "../core/Image.h"
#include "../core/TextLabel.h"

namespace app
{
    TooltipPanel::TooltipPanel(core::AssetManager& assetManager)
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

    void TooltipPanel::Show(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& offset)
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

    void TooltipPanel::Hide()
    {
        SetActive(false);
    }
}