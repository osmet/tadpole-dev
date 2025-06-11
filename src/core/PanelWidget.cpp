#include "Precompiled.h"
#include "PanelWidget.h"

namespace core
{
	PanelWidget::~PanelWidget() = default;

	bool PanelWidget::HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow)
	{
		if (!IsActive())
			return false;

		size_t widgetCount = GetWidgetCount();
		for (size_t index = 0u; index < widgetCount; ++index)
		{
			m_currentRenderWidgetIndex = index;

			if (GetWidget(index)->HandleEvent(event, renderWindow))
				return true;
		}

		return false;
	}

	void PanelWidget::Render(sf::RenderWindow& renderWindow)
	{
		if (!IsActive())
			return;

		size_t widgetCount = GetWidgetCount();
		for (size_t index = 0u; index < widgetCount; ++index)
		{
			m_currentRenderWidgetIndex = index;

			GetWidget(index)->Render(renderWindow);
		}
	}

	void PanelWidget::Reserve(size_t capacity)
	{
		m_widgets.reserve(capacity);
	}

	size_t PanelWidget::GetWidgetCount() const
	{
		return m_widgets.size();
	}

	Widget* PanelWidget::GetWidget(size_t index) const
	{
		if (index < GetWidgetCount())
			return m_widgets[index].get();

		return nullptr;
	}

	size_t PanelWidget::GetCurrentRenderWidgetIndex() const
	{
		return m_currentRenderWidgetIndex;
	}

	void PanelWidget::RemoveWidget(size_t index)
	{
		if (index >= GetWidgetCount())
			return;

		GetWidget(index)->SetParent(nullptr);

		m_widgets.erase(m_widgets.begin() + index);

		OnWidgetRemoved();
	}

	void PanelWidget::OnWidgetAdded()
	{
	}

	void PanelWidget::OnWidgetRemoved()
	{
	}
}