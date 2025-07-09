#include "Precompiled.h"
#include "BackgroundPanel.h"

#include "../core/Image.h"

namespace app
{
    BackgroundPanel::BackgroundPanel()
    {
        auto* backgroundImage = CreateWidget<core::Image>();
        m_backgroundImageId = backgroundImage->GetId();
    }

    void BackgroundPanel::SetBackgroundSize(const sf::Vector2f size)
    {
        if (auto* backgroundImage = FindWidgetById<core::Image>(m_backgroundImageId))
            backgroundImage->SetSize(size);
    }

    void BackgroundPanel::SetBackgroundColor(const sf::Color& color)
    {
        if (auto* backgroundImage = FindWidgetById<core::Image>(m_backgroundImageId))
            backgroundImage->SetColor(color);
    }

    void BackgroundPanel::SetBackgroundTexture(const sf::Texture& texture)
    {
        if (auto* backgroundImage = FindWidgetById<core::Image>(m_backgroundImageId))
            backgroundImage->SetTexture(texture);
    }
}
