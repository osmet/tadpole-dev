#include "Precompiled.h"
#include "BackgroundPanel.h"

namespace app
{
    BackgroundPanel::BackgroundPanel(const sf::Vector2f& size)
    {
        m_backgroundImage = CreateWidget<core::Image>();
        m_backgroundImage->SetSize(size);
    }

    void BackgroundPanel::SetBackgroundColor(const sf::Color& color)
    {
        if (m_backgroundImage)
            m_backgroundImage->SetColor(color);
    }

    void BackgroundPanel::SetBackgroundTexture(const sf::Texture& texture)
    {
        if (m_backgroundImage)
            m_backgroundImage->SetTexture(texture);
    }
}
