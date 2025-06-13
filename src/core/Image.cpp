#include "Precompiled.h"
#include "Image.h"

namespace core
{
	bool Image::HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow)
	{
		return false;
	}

	void Image::Render(sf::RenderWindow& renderWindow)
	{
		if (!IsActive())
			return;

		m_rectangleShape.setSize(GetSize());
		m_rectangleShape.setPosition(CalculateRenderPosition());

		renderWindow.draw(m_rectangleShape);
	}

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
}