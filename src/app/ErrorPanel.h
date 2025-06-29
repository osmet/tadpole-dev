#pragma once

#include "../core/CanvasPanel.h"

namespace core
{
    class AssetManager;
    class TextLabel;
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
        core::TextLabel* m_titleTextLabel = nullptr;
        core::TextLabel* m_descriptionTextLabel = nullptr;

        OnConfirm m_onConfirm;
    };
}