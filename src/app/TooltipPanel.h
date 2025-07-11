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

    class TooltipPanelCommands
    {
    public:
        using Show = std::function<void(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& offset)>;
        using Hide = std::function<void()>;

        void SetShow(Show command);
        void SetHide(Hide command);

        void InvokeShow(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& offset) const;
        void InvokeHide() const;

    private:
        Show m_show;
        Hide m_hide;
    };

    class ITooltipPanelClient
    {
    public:
        virtual ~ITooltipPanelClient() = 0;

        virtual TooltipPanelCommands& GetTooltipPanelCommands() = 0;
    };
}
