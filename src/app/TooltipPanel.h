#pragma once

#include "../core/CanvasPanel.h"

namespace core
{
    class AssetManager;
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
        core::WidgetId m_frameImageId;
        core::WidgetId m_textLabelId;
    };
}
