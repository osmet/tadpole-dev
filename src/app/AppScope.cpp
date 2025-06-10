#include "Precompiled.h"
#include "AppScope.h"

#include "AppContext.h"

namespace app
{
    AppScope::AppScope(AppContext& appContext)
        : m_appContext(appContext)
        , m_gameScope(appContext)
    {
    }

    AppScope::~AppScope()
    {
        m_appContext.GetAppDataSource().Save();
        m_appContext.GetGameDataSource().Save();
    }

    void AppScope::Initialize()
    {
        m_appContext.GetAppDataSource().Load();
        m_appContext.GetGameDataSource().Load();

        m_gameScope.Initialize();
    }

    void AppScope::HandleEvent(const sf::Event& event)
    {
        m_gameScope.HandleEvent(event);
    }

    void AppScope::Update(float deltaTime)
    {
        m_gameScope.Update(deltaTime);
    }

    void AppScope::Render(sf::RenderWindow& renderWindow)
    {
        m_gameScope.Render(renderWindow);
    }
}
