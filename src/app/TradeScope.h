#pragma once

#pragma once

#include "../core/Scope.h"
#include "../app_domain/TradeService.h"
#include "TradeUIView.h"
#include "TradeUIViewModel.h"

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

        tl::expected<void, app_domain::TradeError> BeginTrade(const std::string& playerCharacterId, const std::string& traderCharacterId);

    private:
        TradeUIView m_tradeUIView;
        TradeUIViewModel m_tradeUIViewModel;
    };
}