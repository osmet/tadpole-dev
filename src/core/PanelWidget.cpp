#include "Precompiled.h"
#include "PanelWidget.h"

namespace core
{
	PanelWidget::~PanelWidget() = default;

	const sf::Vector2f& PanelWidget::GetContentSize() const
	{
		return m_contentSize;
	}

	void PanelWidget::OnWidgetAdded()
	{
		SetLayoutDirty();
	}

	void PanelWidget::OnWidgetRemoved()
	{
		SetLayoutDirty();
	}

	void PanelWidget::ValidateState()
	{
		if (m_layoutDirty)
		{
			UpdateLayout(m_contentSize);

			m_layoutDirty = false;
		}
	}

	void PanelWidget::UpdateLayout(sf::Vector2f& contentSize)
	{
		// TODO: add implementation for CanvasPanel
	}

	void PanelWidget::SetLayoutDirty()
	{
		m_layoutDirty = true;
	}
}