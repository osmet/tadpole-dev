#include "Precompiled.h"
#include "TextLabel.h"

namespace core
{
	TextLabel::TextLabel()
	{
		m_text.setFillColor(sf::Color::White);
		m_text.setCharacterSize(14);

		SetSizeToContent(true);
	}

	const sf::Vector2f& TextLabel::GetContentSize() const
	{
		return m_contentSize;
	}

	void TextLabel::SetText(const sf::String& text)
	{
		m_text.setString(text);

		UpdateContentSize();
	}

	void TextLabel::SetFont(const sf::Font& font)
	{
		m_text.setFont(font);

		UpdateContentSize();
	}

	void TextLabel::SetFontSize(std::uint32_t size)
	{
		m_text.setCharacterSize(size);

		UpdateContentSize();
	}

	void TextLabel::SetColor(const sf::Color& color)
	{
		m_text.setFillColor(color);
	}

	void TextLabel::SetOutlineThickness(float thickness)
	{
		m_text.setOutlineThickness(thickness);
	}

	void TextLabel::SetOutlineColor(const sf::Color& color)
	{
		m_text.setOutlineColor(color);
	}

	void TextLabel::OnRender(sf::RenderWindow& renderWindow)
	{
		if (m_text.getString().isEmpty())
			return;

		auto position = GetPosition();
		position.y -= std::round(GetSize().y / 2.f);

		m_text.setPosition(position);

		renderWindow.draw(m_text);
	}

	void TextLabel::UpdateContentSize()
	{
		std::string currentText(m_text.getString());

		m_contentSize.x = m_text.getLocalBounds().width;

		m_text.setString("M"); // HACK: height test string

		m_contentSize.y = m_text.getLocalBounds().height;

		m_text.setString(currentText);
	}
}