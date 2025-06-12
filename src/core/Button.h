#pragma once

#include "PanelWidget.h"

namespace core
{
	class Button : public PanelWidget
	{
	public:
		bool HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow) override;
		void Render(sf::RenderWindow& renderWindow) override;

		bool IsInteractable() const;
		void SetInteractable(bool interactable);

		void SetTexture(const sf::Texture& texture);

		void SetColor(const sf::Color& color);
		void SetNormalColor(const sf::Color& color);
		void SetHoveredColor(const sf::Color& color);
		void SetPressedColor(const sf::Color& color);
		void SetDisabledColor(const sf::Color& color);

		void SetOnClick(std::function<void()> onClick);

	private:
		enum class ButtonState : uint8_t
		{
			Normal = 0u,
			Hovered = 1u,
			Pressed = 2u
		};

		sf::Color GetRenderColor() const;

		ButtonState m_state = ButtonState::Normal;

		bool m_interactable = true;

		sf::Sprite m_sprite;

		sf::Color m_color = sf::Color(255u, 255u, 255u, 255u);
		sf::Color m_normalColor = sf::Color(255u, 255u, 255u, 255u);
		sf::Color m_hoveredColor = sf::Color(245u, 245u, 245u, 255u);
		sf::Color m_pressedColor = sf::Color(200u, 200u, 200u, 255u);
		sf::Color m_disabledColor = sf::Color(200u, 200u, 200u, 128u);

		std::function<void()> m_onClick;
	};
}