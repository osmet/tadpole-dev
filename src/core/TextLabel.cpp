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

	bool TextLabel::HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow)
	{
		return false;
	}

	void TextLabel::Render(sf::RenderWindow& renderWindow)
	{
		if (!IsActive())
			return;

		if (m_text.getString().isEmpty())
			return;

		m_text.setOrigin(0, std::round(GetSize().y / 2));
		m_text.setPosition(CalculateRenderPosition());

		renderWindow.draw(m_text);
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

	void TextLabel::UpdateContentSize()
	{
		auto bounds = m_text.getLocalBounds();

		m_contentSize.x = bounds.width;
		m_contentSize.y = bounds.height;
	}
}