#pragma once

#include "PanelWidget.h"

namespace core
{
	class StackPanel : public PanelWidget
	{
	public:
		enum class Orientation
		{
			Horizontal,
			Vertical
		};

		void SetOrientation(Orientation orientation);
		void SetSpacing(float spacing);

	protected:
		void OnWidgetAdded() override;
		void OnWidgetRemoved() override;

	private:
		void UpdateLayout();

		Orientation m_orientation = Orientation::Horizontal;
		float m_spacing = 10.f;
	};
}
