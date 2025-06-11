#include "Precompiled.h"
#include "StackPanel.h"

namespace core
{
	void StackPanel::SetOrientation(Orientation orientation)
	{
		m_orientation = orientation;

		UpdateLayout();
	}

	void StackPanel::SetSpacing(float spacing)
	{
		m_spacing = spacing;

		UpdateLayout();
	}

	void StackPanel::OnWidgetAdded()
	{
		UpdateLayout();
	}

	void StackPanel::OnWidgetRemoved()
	{
		UpdateLayout();
	}

	void StackPanel::UpdateLayout()
	{
		sf::Vector2f currentPosition;

		size_t widgetCount = GetWidgetCount();
		for (size_t index = 0; index < widgetCount; ++index)
		{
			auto* widget = GetWidget(index);

			auto widgetSize = widget->GetSize();

			widget->SetLocalPosition(currentPosition);

			if (m_orientation == Orientation::Horizontal)
				currentPosition.x += widgetSize.x + m_spacing;
			else
				currentPosition.y += widgetSize.y + m_spacing;
		}
	}
}