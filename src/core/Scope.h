#pragma once

namespace core
{
    class Scope
    {
    public:
        virtual ~Scope() = 0;

        virtual void Initialize() = 0;
        virtual void HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow) = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void Render(sf::RenderWindow& renderWindow) = 0;
    };
}
