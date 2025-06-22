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

        auto result = m_tradeScope.BeginTrade("Player", "Wizard");
        if (result)
            m_activeScope = std::ref(m_tradeScope);
        else
            m_activeScope.reset();
    }

    void GameScope::HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow)
    {
        if (m_activeScope)
            m_activeScope->get().HandleEvent(event, renderWindow);
    }

    void GameScope::Update(float deltaTime)
    {
        if (m_activeScope)
            m_activeScope->get().Update(deltaTime);
    }

    void GameScope::Render(sf::RenderWindow& renderWindow)
    {
        m_backgroundPanel->Render(renderWindow);

        if (m_activeScope)
            m_activeScope->get().Render(renderWindow);
    }
}