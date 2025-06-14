#pragma once

#include "Widget.h"

namespace core
{
	class Image : public Widget
	{
	public:
		void SetTexture(const sf::Texture& texture);
		void SetColor(const sf::Color& color);
		void SetOutlineThickness(float thickness);
		void SetOutlineColor(const sf::Color& color);

	protected:
		void OnRender(sf::RenderWindow& renderWindow) override;

	private:
		sf::RectangleShape m_rectangleShape;
	};
}