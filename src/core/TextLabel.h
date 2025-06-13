#pragma once

#include "Widget.h"

namespace core
{
	class TextLabel : public Widget
	{
	public:
		TextLabel();

		bool HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow) override;
		void Render(sf::RenderWindow& renderWindow) override;

		const sf::Vector2f& GetContentSize() const override;

		void SetText(const sf::String& text);
		void SetFont(const sf::Font& font);
		void SetFontSize(std::uint32_t size);
		void SetColor(const sf::Color& color);
		void SetOutlineThickness(float thickness);
		void SetOutlineColor(const sf::Color& color);

	private:
		void UpdateContentSize();

		sf::Vector2f m_contentSize;

		sf::Text m_text;
	};
}
