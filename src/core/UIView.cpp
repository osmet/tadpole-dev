#include "Precompiled.h"
#include "UIView.h"

namespace core
{
	UIView::~UIView() = default;

	void UIView::HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow)
	{
		if (m_mainWidget)
			m_mainWidget->HandleEvent(event, renderWindow);
	}

	void UIView::Update(float deltaTime)
	{
	}

	void UIView::Render(sf::RenderWindow& renderWindow)
	{
		if (m_mainWidget)
			m_mainWidget->Render(renderWindow);
	}
}