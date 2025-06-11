#include "Precompiled.h"
#include "Button.h"

namespace core
{
	bool Button::HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow)
	{
		if (!IsActive() || !m_interactable)
		{
			m_state = ButtonState::Normal;
			return false;
		}

		sf::FloatRect buttonBounds(CalculatePosition(), GetSize());
		sf::Vector2f mousePosition(sf::Mouse::getPosition(renderWindow));

		bool isHovered = buttonBounds.contains(mousePosition);

		if (!isHovered)
		{
			m_state = ButtonState::Normal;

			return false;
		}

		if (event.type == sf::Event::MouseButtonPressed 
			&& event.mouseButton.button == sf::Mouse::Left)
		{
			m_state = ButtonState::Pressed;

			return true;
		}

		if (event.type == sf::Event::MouseButtonReleased 
			&& event.mouseButton.button == sf::Mouse::Left
			&& m_state == ButtonState::Pressed)
		{
			if (m_onClick)
				m_onClick();

			m_state = ButtonState::Hovered;

			return true;
		}

		m_state = ButtonState::Hovered;

		return false;
	}

	void Button::Render(sf::RenderWindow& renderWindow)
	{
		if (!IsActive())
			return;

		const auto position = CalculatePosition();
		const auto size = GetSize();
		const auto color = GetColor();

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

		PanelWidget::Render(renderWindow);
	}

	bool Button::IsInteractable() const
	{
		return m_interactable;
	}

	void Button::SetInteractable(bool interactable)
	{
		m_interactable = interactable;
	}

	void Button::SetTexture(const sf::Texture& texture)
	{
		m_sprite.setTexture(texture, true);
	}

	void Button::SetNormalColor(const sf::Color& color)
	{
		m_normalColor = color;
	}

	void Button::SetHoveredColor(const sf::Color& color)
	{
		m_hoveredColor = color;
	}

	void Button::SetPressedColor(const sf::Color& color)
	{
		m_pressedColor = color;
	}

	void Button::SetDisabledColor(const sf::Color& color)
	{
		m_disabledColor = color;
	}

	void Button::SetOnClick(std::function<void()> onClick)
	{
		m_onClick = std::move(onClick);
	}

	sf::Color Button::GetColor() const
	{
		if (!m_interactable)
			return m_disabledColor;

		switch (m_state)
		{
		case ButtonState::Normal: return m_normalColor;
		case ButtonState::Hovered: return m_hoveredColor;
		case ButtonState::Pressed: return m_pressedColor;
		default: return m_normalColor;
		}
	}
}
