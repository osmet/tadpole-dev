#include "Precompiled.h"
#include "TradeScope.h"

#include "AppContext.h"

namespace app
{
    TradeScope::TradeScope(AppContext& appContext)
        : m_tradeService(appContext.GetTradeService())
        , m_tradeUIView(appContext)
    {
    }

    void TradeScope::Initialize()
    {
        m_tradeUIView.Initialize();
    }

    void TradeScope::HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow)
    {
        m_tradeUIView.HandleEvent(event, renderWindow);
    }

    void TradeScope::Update(float deltaTime)
    {
        m_tradeUIView.Update(deltaTime);
    }

    void TradeScope::Render(sf::RenderWindow& renderWindow)
    {
        m_tradeUIView.Render(renderWindow);
    }
}
