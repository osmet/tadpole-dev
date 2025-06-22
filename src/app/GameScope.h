#pragma once

#include "BackgroundPanel.h"
#include "TradeScope.h"

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

        std::unique_ptr<BackgroundPanel> m_backgroundPanel;

        TradeScope m_tradeScope;

        std::optional<std::reference_wrapper<core::Scope>> m_activeScope;
    };
}
