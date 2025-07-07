#pragma once

#include "BackgroundPanel.h"
#include "TradeScope.h"
#include "../core/TextLabel.h"

namespace app
{
    class AppContext;
    class BackgroundPanel;

    class GameScope : public core::Scope
    {
    public:
        GameScope(AppContext& appContext);

        void Initialize() override;
        void HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow) override;
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& renderWindow) override;

    private:
        AppContext& m_appContext;

        BackgroundPanel m_backgroundPanel;
        core::TextLabel m_lastSavedAtTextLabel;

        TradeScope m_tradeScope;

        lang::OptionalRef<core::Scope> m_activeScope;
    };
}
