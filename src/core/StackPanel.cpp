#include "Precompiled.h"
#include "StackPanel.h"

namespace core
{
	StackPanel::StackPanel()
	{
		SetSizeToContent(true);
	}

	void StackPanel::SetOrientation(Orientation orientation)
	{
		m_orientation = orientation;

		SetLayoutDirty();
	}

	void StackPanel::SetSpacing(float spacing)
	{
		m_spacing = spacing;

		SetLayoutDirty();
	}

	void StackPanel::UpdateLayout(sf::Vector2f& out_сontentSize)
	{
		sf::Vector2f contentSize;

		sf::Vector2f currentPosition;
		
		size_t widgetCount = GetWidgetCount();
		for (size_t index = 0; index < widgetCount; ++index)
		{
			auto* widget = GetWidget(index);

			auto widgetSize = widget->GetSize();

			widget->SetLocalPosition(currentPosition);

			if (m_orientation == Orientation::Horizontal)
			{
				float offset = widgetSize.x + m_spacing;

				currentPosition.x += offset;

				contentSize.x += offset;

				if (widgetSize.y > contentSize.y)
					contentSize.y = widgetSize.y;
			}
			else
			{
				float offset = widgetSize.y + m_spacing;

				currentPosition.y += offset;

				contentSize.y += offset;

				if (widgetSize.x > contentSize.x)
					contentSize.x = widgetSize.x;
			}
		}

		if (m_orientation == Orientation::Horizontal)
			contentSize.x -= m_spacing;
		else
			contentSize.y -= m_spacing;

		out_сontentSize.x = contentSize.x;
		out_сontentSize.y = contentSize.y;
	}
}