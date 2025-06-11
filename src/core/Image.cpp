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

		const auto position = CalculatePosition();
		const auto size = GetSize();
		const auto color = m_color;

		if (m_sprite.getTexture())
		{
			const auto& textureRect = m_sprite.getTextureRect();

			if (textureRect.width == 0 || textureRect.height == 0)
				return;

			m_sprite.setPosition(position);
			m_sprite.setScale(
				size.x / static_cast<float>(textureRect.width),
				size.y / static_cast<float>(textureRect.height)
			);
			m_sprite.setColor(color);

			renderWindow.draw(m_sprite);
		}
		else
		{
			sf::RectangleShape rect(size);

			rect.setPosition(position);
			rect.setFillColor(color);

			renderWindow.draw(rect);
		}
	}

	void Image::SetTexture(const sf::Texture& texture)
	{
		m_sprite.setTexture(texture);
	}

	void Image::SetColor(const sf::Color& color)
	{
		m_color = color;
	}
}