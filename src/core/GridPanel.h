#pragma once

#include "PanelWidget.h"

namespace core
{
	class GridPanel : public PanelWidget
	{
	public:
		GridPanel();

		void SetColumnCount(size_t columnCount);
		void SetCellSize(const sf::Vector2f& cellSize);
		void SetCellSize(float x, float y);
		void SetCellSize(float cellSize);
		void SetSpacing(const sf::Vector2f& spacing);
		void SetSpacing(float x, float y);
		void SetSpacing(float spacing);

	protected:
		void UpdateLayout(sf::Vector2f& out_сontentSize) override;

	private:
		size_t m_columnCount = 2;
		sf::Vector2f m_cellSize = { 100.f, 100.f };
		sf::Vector2f m_spacing = { 10.f, 10.f };
	};
}
