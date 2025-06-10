#pragma once

#include "../core/Scope.h"
#include "TradeScope.h"

namespace app
{
    class AppContext;

    class GameScope : public core::Scope
    {
    public:
        GameScope(AppContext& appContext);

        void Initialize() override;
        void HandleEvent(const sf::Event& event) override;
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& renderWindow) override;

    private:
        TradeScope m_tradeScope;
    };
}
