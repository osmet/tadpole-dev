#pragma once

#include "../core/CanvasPanel.h"

namespace core
{
    class Image;
}

namespace app
{
    class ScreenOverlayPanel final : public core::CanvasPanel
    {
    public:
        ScreenOverlayPanel(const sf::Vector2f& renderWindowSize);

        void Show();
        void Hide();
    };
}
