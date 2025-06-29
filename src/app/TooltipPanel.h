#pragma once

#include "../core/CanvasPanel.h"

namespace core
{
    class AssetManager;
    class Image;
    class TextLabel;
}

namespace app
{
    class TooltipPanel final : public core::CanvasPanel
    {
    public:
        TooltipPanel(core::AssetManager& assetManager);

        void Show(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& offset);
        void Hide();
    private:
        core::Image* m_frameImage = nullptr;
        core::TextLabel* m_textLabel = nullptr;
    };
}
