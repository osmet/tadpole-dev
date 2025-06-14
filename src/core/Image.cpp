#include "Precompiled.h"
#include "Image.h"

namespace core
{
	void Image::SetTexture(const sf::Texture& texture)
	{
		m_rectangleShape.setTexture(&texture);
	}

	void Image::SetColor(const sf::Color& color)
	{
		m_rectangleShape.setFillColor(color);
	}

	void Image::SetOutlineThickness(float thickness)
	{
		m_rectangleShape.setOutlineThickness(thickness);
	}

	void Image::SetOutlineColor(const sf::Color& color)
	{
		m_rectangleShape.setOutlineColor(color);
	}

	void Image::OnRender(sf::RenderWindow& renderWindow)
	{
		m_rectangleShape.setSize(GetSize());
		m_rectangleShape.setPosition(CalculateRenderPosition());

		renderWindow.draw(m_rectangleShape);
	}
}