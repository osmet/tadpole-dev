#include "Precompiled.h"
#include "Widget.h"

namespace core
{
	Widget::~Widget() = default;

	const sf::String& Widget::GetName() const
	{
		return m_name;
	}

	void Widget::SetName(const sf::String& name)
	{
		m_name = name;
	}

	bool Widget::IsActive() const
	{
		return m_active;
	}

	void Widget::SetActive(bool active)
	{
		m_active = active;
	}

	const sf::Vector2f& Widget::GetAnchor() const
	{
		return m_anchor;
	}

	void Widget::SetAnchor(const sf::Vector2f& anchor)
	{
		m_anchor = anchor;
	}

	void Widget::SetAnchor(float x, float y)
	{
		m_anchor.x = x;
		m_anchor.y = y;
	}

	const sf::Vector2f& Widget::GetPivot() const
	{
		return m_pivot;
	}

	void Widget::SetPivot(const sf::Vector2f& pivot)
	{
		m_pivot = pivot;
	}

	void Widget::SetPivot(float x, float y)
	{
		m_pivot.x = x;
		m_pivot.y = y;
	}

	const sf::Vector2f& Widget::GetLocalPosition() const
	{
		return m_localPosition;
	}

	void Widget::SetLocalPosition(const sf::Vector2f& localPosition)
	{
		m_localPosition = localPosition;
	}

	void Widget::SetLocalPosition(float x, float y)
	{
		m_localPosition.x = x;
		m_localPosition.y = y;
	}

	const sf::Vector2f& Widget::GetSize() const
	{
		return IsSizeToContent() ? GetContentSize() : m_size;
	}

	void Widget::SetSize(const sf::Vector2f& size)
	{
		m_size = size;
	}

	void Widget::SetSize(float x, float y)
	{
		m_size.x = x;
		m_size.y = y;
	}

	void Widget::SetSize(float size)
	{
		m_size.x = size;
		m_size.y = size;
	}

	bool Widget::IsSizeToContent() const
	{
		return m_sizeToContent;
	}

	void Widget::SetSizeToContent(bool sizeToContent)
	{
		m_sizeToContent = sizeToContent;
	}

	const sf::Vector2f& Widget::GetContentSize() const
	{
		return m_size;
	}

	const Widget* Widget::GetParent() const
	{
		return m_parent;
	}

	sf::Vector2f Widget::CalculateRenderPosition() const
	{
		const auto* parentWidget = GetParent();

		sf::Vector2f parentPosition;
		sf::Vector2f parentSize;

		if (parentWidget != nullptr)
		{
			parentPosition = parentWidget->CalculateRenderPosition();
			parentSize = parentWidget->GetSize();
		}

		auto& size = GetSize();

		float rawPositionX = parentPosition.x + m_anchor.x * parentSize.x + m_localPosition.x - size.x * m_pivot.x;
		float rawPositionY = parentPosition.y + m_anchor.y * parentSize.y + m_localPosition.y - size.y * m_pivot.y;

		return sf::Vector2f(std::round(rawPositionX), std::round(rawPositionY));
	}

	void Widget::SetParent(Widget* widget)
	{
		m_parent = widget;
	}
}