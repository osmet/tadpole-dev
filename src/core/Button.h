#pragma once

#include "PanelWidget.h"

namespace core
{
	class Button : public PanelWidget
	{
	public:
		using OnClick = std::function<void()>;
		using OnHoverIn = std::function<void(const sf::Vector2f& mousePosition)>;
		using OnHoverOut = std::function<void()>;

		bool IsInteractable() const;
		void SetInteractable(bool interactable);

		void SetTexture(const sf::Texture& texture);

		void SetColor(const sf::Color& color);
		void SetNormalColor(const sf::Color& color);
		void SetHoveredColor(const sf::Color& color);
		void SetPressedColor(const sf::Color& color);
		void SetDisabledColor(const sf::Color& color);
		void SetOutlineThickness(float thickness);
		void SetOutlineColor(const sf::Color& color);

		void SetOnClick(OnClick callback);
		void SetOnHoverIn(OnHoverIn callback);
		void SetOnHoverOut(OnHoverOut callback);

	protected:
		bool OnHandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow) override;
		void OnRender(sf::RenderWindow& renderWindow) override;

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

		sf::RectangleShape m_rectangleShape;

		sf::Color m_color = sf::Color(255u, 255u, 255u, 255u);
		sf::Color m_normalColor = sf::Color(255u, 255u, 255u, 255u);
		sf::Color m_hoveredColor = sf::Color(245u, 245u, 245u, 255u);
		sf::Color m_pressedColor = sf::Color(200u, 200u, 200u, 255u);
		sf::Color m_disabledColor = sf::Color(200u, 200u, 200u, 128u);

		OnClick m_onClick;
		OnHoverIn m_onHoverIn;
		OnHoverOut m_onHoverOut;
	};
}