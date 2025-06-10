#include "Precompiled.h"
#include "GameScope.h"

#include "AppContext.h"

namespace app
{
    GameScope::GameScope(AppContext& appContext)
        : m_tradeScope(appContext)
    {
    }

    void GameScope::Initialize()
    {
        m_tradeScope.Initialize();
    }

    void GameScope::HandleEvent(const sf::Event& event)
    {
        m_tradeScope.HandleEvent(event);
    }

    void GameScope::Update(float deltaTime)
    {
        m_tradeScope.Update(deltaTime);
    }

    void GameScope::Render(sf::RenderWindow& renderWindow)
    {
        m_tradeScope.Render(renderWindow);
    }
}
