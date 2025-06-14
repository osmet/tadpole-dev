#include "Precompiled.h"
#include "Button.h"

namespace core
{
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
		m_rectangleShape.setTexture(&texture);
	}

	void Button::SetColor(const sf::Color& color)
	{
		m_color = color;
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
	void Button::SetOutlineThickness(float thickness)
	{
		m_rectangleShape.setOutlineThickness(thickness);
	}

	void Button::SetOutlineColor(const sf::Color& color)
	{
		m_rectangleShape.setOutlineColor(color);
	}

	void Button::SetOnClick(std::function<void()> onClick)
	{
		m_onClick = std::move(onClick);
	}

	bool Button::OnHandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow)
	{
		if (!m_interactable)
		{
			m_state = ButtonState::Normal;

			return false;
		}

		sf::FloatRect buttonBounds(CalculateRenderPosition(), GetSize());
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

		if (m_state == ButtonState::Pressed
			&& sf::Mouse::isButtonPressed(sf::Mouse::Left))
			return false;

		m_state = ButtonState::Hovered;

		return true;
	}

	void Button::OnRender(sf::RenderWindow& renderWindow)
	{
		m_rectangleShape.setPosition(CalculateRenderPosition());
		m_rectangleShape.setSize(GetSize());
		m_rectangleShape.setFillColor(GetRenderColor());

		renderWindow.draw(m_rectangleShape);
	}

	sf::Color Button::GetRenderColor() const
	{
		sf::Color stateColor = !m_interactable
			? m_disabledColor
			: (m_state == ButtonState::Hovered ? m_hoveredColor :
				m_state == ButtonState::Pressed ? m_pressedColor :
				m_normalColor);

		return {
			static_cast<sf::Uint8>((stateColor.r * m_color.r) / 255),
			static_cast<sf::Uint8>((stateColor.g * m_color.g) / 255),
			static_cast<sf::Uint8>((stateColor.b * m_color.b) / 255),
			static_cast<sf::Uint8>((stateColor.a * m_color.a) / 255)
		};
	}
}