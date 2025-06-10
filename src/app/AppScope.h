#pragma once

#include "../core/Scope.h"
#include "GameScope.h"
#include "AppContext.h"

namespace app
{
    class AppContext;

    class AppScope : public core::Scope
    {
    public:
        AppScope(AppContext& appContext);
        ~AppScope();

        void Initialize() override;
        void HandleEvent(const sf::Event& event) override;
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& renderWindow) override;

    private:
        AppContext& m_appContext;
        GameScope m_gameScope;
    };
}
