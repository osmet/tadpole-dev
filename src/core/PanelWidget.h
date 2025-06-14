#pragma once

#include "Widget.h"

namespace core
{
	class PanelWidget : public Widget
	{
	public:
		virtual ~PanelWidget() = 0;

		virtual const sf::Vector2f& GetContentSize() const;

	protected:
		void OnWidgetAdded() override;
		void OnWidgetRemoved() override;

		void ValidateState() override;

		virtual void UpdateLayout(sf::Vector2f& out_contentSize);
		void SetLayoutDirty();

	private:
		sf::Vector2f m_contentSize;
		bool m_layoutDirty = false;

	};
}