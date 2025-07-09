#pragma once

#include "../core/CanvasPanel.h"

namespace core
{
    class AssetManager;
}

namespace app
{
    class ScreenOverlayPanel;

    class ErrorPanel final : public core::CanvasPanel
    {
    public:
        using OnConfirm = std::function<void()>;

        ErrorPanel(core::AssetManager& assetManager, const sf::Vector2f& renderWindowSize);

        void Show(const std::string& title, const std::string& description);

        void SetOnConfirm(OnConfirm callback);

    private:
        core::WidgetId m_titleTextLabelId;
        core::WidgetId m_descriptionTextLabelId;

        OnConfirm m_onConfirm;
    };
}