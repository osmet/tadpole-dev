#pragma once

#include "PanelWidget.h"

namespace core
{
    class UIView
    {
    public:
        virtual ~UIView() = 0;

        virtual void Initialize() = 0;

        virtual void HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow);
        virtual void Update(float deltaTime);
        virtual void Render(sf::RenderWindow& renderWindow);

    protected:
        std::unique_ptr<PanelWidget> m_mainWidget;
    };
}
