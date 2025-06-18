#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

namespace core
{
	class Widget
	{
	public:
		virtual ~Widget() = 0;

		bool HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow);
		void Render(sf::RenderWindow& renderWindow);

		const sf::String& GetName() const;
		void SetName(const sf::String& name);

		bool IsActiveSelf() const;
		bool IsActiveInHierarchy() const;
		void SetActive(bool active);

		const sf::Vector2f& GetAnchor() const;
		void SetAnchor(const sf::Vector2f& anchor);
		void SetAnchor(float x, float y);

		const sf::Vector2f& GetPivot() const;
		void SetPivot(const sf::Vector2f& pivot);
		void SetPivot(float x, float y);

		const sf::Vector2f& GetLocalPosition() const;
		void SetLocalPosition(const sf::Vector2f& localPosition);
		void SetLocalPosition(float x, float y);
		sf::Vector2f GetPosition() const;

		const sf::Vector2f& GetSize() const;
		void SetSize(const sf::Vector2f& size);
		void SetSize(float x, float y);
		void SetSize(float size);
		bool IsSizeToContent() const;
		void SetSizeToContent(bool sizeToContent);
		virtual const sf::Vector2f& GetContentSize() const;
		
		void Reserve(size_t capacity);
		size_t GetWidgetCount() const;
		Widget* GetWidget(size_t index) const;

		template<class TWidget, typename... Args>
		TWidget* CreateWidget(Args&&... args)
		{
			auto widget = std::make_unique<TWidget>(std::forward<Args>(args)...);
			widget->SetParent(this);

			TWidget* rawPointer = widget.get();

			m_widgets.push_back(std::move(widget));

			OnWidgetAdded();

			return rawPointer;
		}

		void RemoveWidget(size_t index);

		const Widget* GetParent() const;

	protected:
		virtual bool OnHandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow);
		virtual void OnRender(sf::RenderWindow& renderWindow);

		virtual void OnWidgetAdded();
		virtual void OnWidgetRemoved();

		virtual void ValidateState();

	private:
		void SetParent(Widget* widget);

		sf::String m_name = "Widget";
		bool m_active = true;
		sf::Vector2f m_anchor = { 0.5f, 0.5f };
		sf::Vector2f m_pivot = { 0.5f, 0.5f };
		sf::Vector2f m_localPosition;
		sf::Vector2f m_size = { 100.f, 100.f };
		bool m_sizeToContent = false;

		Widget* m_parent = nullptr;
		std::vector<std::unique_ptr<Widget>> m_widgets;
	};
}