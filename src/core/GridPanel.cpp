#include "Precompiled.h"
#include "GridPanel.h"

namespace core
{
	void GridPanel::SetColumnCount(size_t columnCount)
	{
		m_columnCount = std::max(static_cast<size_t>(1), columnCount);

		UpdateLayout();
	}

	void GridPanel::SetCellSize(const sf::Vector2f& cellSize)
	{
		m_cellSize = cellSize;

		UpdateLayout();
	}

	void GridPanel::SetCellSize(float x, float y)
	{
		m_cellSize.x = x;
		m_cellSize.y = y;

		UpdateLayout();
	}

	void GridPanel::SetSpacing(float spacing)
	{
		m_spacing = spacing;

		UpdateLayout();
	}

	void GridPanel::OnWidgetAdded()
	{
		UpdateLayout();
	}

	void GridPanel::OnWidgetRemoved()
	{
		UpdateLayout();
	}

	void GridPanel::UpdateLayout()
	{
		const size_t widgetCount = GetWidgetCount();
		for (size_t index = 0; index < widgetCount; ++index)
		{
			auto* widget = GetWidget(index);

			size_t row = index / m_columnCount;
			size_t column = index % m_columnCount;

			sf::Vector2f position{
				column * (m_cellSize.x + m_spacing),
				row * (m_cellSize.y + m_spacing)
			};

			widget->SetSize(m_cellSize);
			widget->SetLocalPosition(position);
		}
	}
}
