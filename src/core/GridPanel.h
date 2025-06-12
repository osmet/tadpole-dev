#pragma once

#include "PanelWidget.h"

namespace core
{
	class GridPanel : public PanelWidget
	{
	public:
		void SetColumnCount(size_t columnCount);
		void SetCellSize(const sf::Vector2f& cellSize);
		void SetCellSize(float x, float y);
		void SetSpacing(float spacing);

	protected:
		void OnWidgetAdded() override;
		void OnWidgetRemoved() override;

	private:
		void UpdateLayout();

		size_t m_columnCount = 2;
		sf::Vector2f m_cellSize = { 100.f, 100.f };
		float m_spacing = 10.f;
	};
}
