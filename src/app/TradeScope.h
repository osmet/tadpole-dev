#pragma once

#pragma once

#include "../core/Scope.h"
#include "../app_domain/TradeService.h"
#include "TradeUIView.h"

namespace app
{
    class AppContext;

    class TradeScope : public core::Scope
    {
    public:
        TradeScope(AppContext& appContext);

        void Initialize() override;
        void HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow) override;
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& renderWindow) override;

    private:
        app_domain::TradeService& m_tradeService;
        TradeUIView m_tradeUIView;
    };
}