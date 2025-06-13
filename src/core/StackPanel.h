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

		StackPanel();

		void SetOrientation(Orientation orientation);
		void SetSpacing(float spacing);

	protected:
		void UpdateLayout(sf::Vector2f& out_сontentSize) override;

	private:
		Orientation m_orientation = Orientation::Horizontal;
		float m_spacing = 10.f;
	};
}
