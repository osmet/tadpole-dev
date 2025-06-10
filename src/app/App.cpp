#include "Precompiled.h"
#include "App.h"
#include "AppConstants.h"

namespace app
{
    App::App()
        : m_appScope(m_appContext)
    {
    }

    void App::Initialize()
    {
        m_appContext.Initialize();

        const AppConfig& config = m_appContext.GetAppConfig();

        m_renderWindow.create(
            sf::VideoMode(config.GetAppWindowWidth(), config.GetAppWindowHeight()),
            config.GetAppName(),
            config.GetAppWindowStyle()
        );

        m_appScope.Initialize();
    }

    void App::Run()
    {
        while (m_renderWindow.isOpen())
        {
            float deltaTime = m_frameClock.restart().asSeconds();

            HandleEvents();
            m_appScope.Update(deltaTime);
            Render();
        }
    }

    void App::HandleEvents()
    {
        sf::Event event;
        while (m_renderWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                m_renderWindow.close();
            }

            m_appScope.HandleEvent(event);
        }
    }

    void App::Render()
    {
        m_renderWindow.clear();

        m_appScope.Render(m_renderWindow);

        m_renderWindow.display();
    }
}
