#pragma once

#include "AppContext.h"
#include "AppScope.h"

namespace app
{
    class App
    {
    public:
        App();

        void Initialize();
        void Run();

    private:
        void HandleEvents();
        void Render();

        sf::Clock m_frameClock;
        sf::RenderWindow m_renderWindow;

        AppContext m_appContext;
        AppScope m_appScope;
    };
}
