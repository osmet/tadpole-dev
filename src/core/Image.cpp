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

		const auto position = CalculateRenderPosition();
		const auto size = GetSize();
		const auto color = m_color;

		if (m_sprite.getTexture())
		{
			auto bounds = m_sprite.getLocalBounds();

			if (bounds.width == 0.f || bounds.height == 0.f)
				return;

			m_sprite.setPosition(position);
			m_sprite.setScale(
				size.x / bounds.width,
				size.y / bounds.height
			);
			m_sprite.setColor(color);

			renderWindow.draw(m_sprite);
		}
		else
		{
			sf::RectangleShape rect(size);

			rect.setPosition(position);
			rect.setSize(size);
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