#pragma once

#include "Widget.h"

namespace core
{
	class PanelWidget : public Widget
	{
	public:
		virtual ~PanelWidget() = 0;

		bool HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow) override;
		void Render(sf::RenderWindow& renderWindow) override;

		void Reserve(size_t capacity);
		size_t GetWidgetCount() const;
		Widget* GetWidget(size_t index) const;
		size_t GetCurrentRenderWidgetIndex() const;

		template<class TWidget, typename... Args>
		TWidget* CreateWidget(Args&&... args)
		{
			auto widget = std::make_unique<TWidget>(std::forward<Args>(args)...);
			widget->SetParent(this);

			auto* rawPointer = widget.get();
			m_widgets.push_back(std::move(widget));

			OnWidgetAdded();

			return rawPointer;
		}

		void RemoveWidget(size_t index);

	protected:
		virtual void OnWidgetAdded();
		virtual void OnWidgetRemoved();

	private:
		std::vector<std::unique_ptr<Widget>> m_widgets;
		size_t m_currentRenderWidgetIndex = 0u;
	};
}