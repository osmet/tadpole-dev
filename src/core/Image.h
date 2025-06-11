#pragma once

#include "Widget.h"

namespace core
{
	class Image : public Widget
	{
	public:
		bool HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow) override;
		void Render(sf::RenderWindow& renderWindow) override;

		void SetTexture(const sf::Texture& texture);
		void SetColor(const sf::Color& color);

	private:
		sf::Sprite m_sprite;
		sf::Color m_color = sf::Color::White;
	};
}