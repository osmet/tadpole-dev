#pragma once

#include "../core/Scope.h"
#include "GameScope.h"

namespace app
{
    class AppContext;

    class AppScope : public core::Scope
    {
    public:
        AppScope(AppContext& appContext);
        ~AppScope();

        void Initialize() override;
        void HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow) override;
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& renderWindow) override;

    private:
        AppContext& m_appContext;
        GameScope m_gameScope;
    };
}
