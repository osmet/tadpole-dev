#include "Precompiled.h"
#include "GridPanel.h"

namespace core
{
	GridPanel::GridPanel()
	{
		SetSizeToContent(true);
	}

	void GridPanel::SetColumnCount(size_t columnCount)
	{
		m_columnCount = std::max(static_cast<size_t>(1), columnCount);

		SetLayoutDirty();
	}

	void GridPanel::SetCellSize(const sf::Vector2f& cellSize)
	{
		m_cellSize = cellSize;

		SetLayoutDirty();
	}

	void GridPanel::SetCellSize(float x, float y)
	{
		m_cellSize.x = x;
		m_cellSize.y = y;

		SetLayoutDirty();
	}

	void GridPanel::SetCellSize(float cellSize)
	{
		m_cellSize.x = cellSize;
		m_cellSize.y = cellSize;

		SetLayoutDirty();
	}

	void GridPanel::SetSpacing(const sf::Vector2f& spacing)
	{
		m_spacing = spacing;

		SetLayoutDirty();
	}

	void GridPanel::SetSpacing(float x, float y)
	{
		m_spacing.x = x;
		m_spacing.y = y;

		SetLayoutDirty();
	}

	void GridPanel::SetSpacing(float spacing)
	{
		m_spacing.x = spacing;
		m_spacing.y = spacing;

		SetLayoutDirty();
	}

	void GridPanel::UpdateLayout(sf::Vector2f& out_сontentSize)
	{
		const size_t widgetCount = GetWidgetCount();
		for (size_t index = 0; index < widgetCount; ++index)
		{
			auto* widget = GetWidget(index);

			size_t row = index / m_columnCount;
			size_t column = index % m_columnCount;

			sf::Vector2f position{
				column * (m_cellSize.x + m_spacing.x),
				row * (m_cellSize.y + m_spacing.y)
			};

			widget->SetSize(m_cellSize);
			widget->SetLocalPosition(position);
		}
		
		size_t rowCount = widgetCount / m_columnCount;

		out_сontentSize.x = m_columnCount * m_cellSize.x + (m_columnCount - 1) * m_spacing.x;
		out_сontentSize.y = rowCount * m_cellSize.x + (rowCount - 1) * m_spacing.x;
	}
}
