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
        : m_tradeUIViewModel(appContext)
        , m_tradeUIView(appContext, m_tradeUIViewModel)
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

    tl::expected<void, app_domain::TradeError> TradeScope::BeginTrade(const std::string& playerCharacterId, const std::string& traderCharacterId)
    {
        return m_tradeUIViewModel.BeginTrade(playerCharacterId, traderCharacterId);
    }
}
