#include "Precompiled.h"
#include "TradeScope.h"

#include "AppContext.h"

namespace app
{
    TradeScope::TradeScope(AppContext& appContext)
        : m_tradeService(appContext.GetTradeService())
    {
    }

    void TradeScope::Initialize()
    {
    }

    void TradeScope::HandleEvent(const sf::Event& event)
    {
    }

    void TradeScope::Update(float deltaTime)
    {
    }

    void TradeScope::Render(sf::RenderWindow& renderWindow)
    {
    }
}
