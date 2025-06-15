#include "Precompiled.h"
#include "TradeScope.h"
#include "AppContext.h"
#include "../app_domain/CharacterService.h"
#include "../app_domain/ItemService.h"
#include "../app_domain/InventoryService.h"
#include "../app_domain/TradeService.h"

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

        m_tradeUIView.SetPlayerName("Alice");
        m_tradeUIView.SetPlayerPortraitTexture("UI_Portrait_Alice");
        m_tradeUIView.SetPlayerMoney(1530);
        m_tradeUIView.SetPlayerWeight(50.5f, 150.f);

        m_tradeUIView.SetTraderName("Gale");
        m_tradeUIView.SetTraderPortraitTexture("UI_Portrait_Gale");
        m_tradeUIView.SetTraderMoney(350);
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
