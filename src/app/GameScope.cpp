#include "Precompiled.h"
#include "GameScope.h"

#include "AppContext.h"
#include "BackgroundPanel.h"

namespace app
{
    GameScope::GameScope(AppContext& appContext)
        : m_appContext(appContext)
        , m_tradeScope(appContext)
    {
    }

    void GameScope::Initialize()
    {
        auto renderWindowSize = m_appContext.GetRenderWindowSize();
        m_backgroundPanel = std::make_unique<BackgroundPanel>(renderWindowSize);
        m_backgroundPanel->SetLocalPosition(renderWindowSize.x / 2.f, renderWindowSize.y / 2.f);
        m_backgroundPanel->SetBackgroundTexture(m_appContext.GetAssetManager().GetTexture("Background_Dialogue_Gale"));
        sf::Uint8 backgroundTint(static_cast<sf::Uint8>(0.2f * 255u));
        m_backgroundPanel->SetBackgroundColor(sf::Color(backgroundTint, backgroundTint, backgroundTint, 255u));

        m_tradeScope.Initialize();
    }

    void GameScope::HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow)
    {
        m_tradeScope.HandleEvent(event, renderWindow);
    }

    void GameScope::Update(float deltaTime)
    {
        m_tradeScope.Update(deltaTime);
    }

    void GameScope::Render(sf::RenderWindow& renderWindow)
    {
        m_backgroundPanel->Render(renderWindow);

        m_tradeScope.Render(renderWindow);
    }
}
