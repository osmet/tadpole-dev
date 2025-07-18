#pragma once

#include "../core/CanvasPanel.h"

namespace app
{
    class BackgroundPanel final : public core::CanvasPanel
    {
    public:
        BackgroundPanel();

        void SetBackgroundSize(const sf::Vector2f size);
        void SetBackgroundColor(const sf::Color& color);
        void SetBackgroundTexture(const sf::Texture& texture);

    private:
        core::WidgetId m_backgroundImageId;
    };
}