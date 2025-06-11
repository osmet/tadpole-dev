#pragma once

namespace core
{
	class Widget
	{
	public:
		virtual ~Widget() = 0;

		virtual bool HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow) = 0;
		virtual void Render(sf::RenderWindow& renderWindow) = 0;

		const sf::String& GetName() const;
		void SetName(const sf::String& name);

		bool IsActive() const;
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

		const sf::Vector2f& GetSize() const;
		void SetSize(const sf::Vector2f& size);
		void SetSize(float x, float y);
		bool IsSizeToContent() const;
		void SetSizeToContent(bool sizeToContent);
		virtual const sf::Vector2f& GetContentSize() const;

		const Widget* GetParent() const;

	protected:
		sf::Vector2f CalculatePosition() const;

	private:
		friend class PanelWidget;

		void SetParent(Widget* widget);

		Widget* m_parent = nullptr;

		sf::String m_name = "Widget";
		bool m_active = true;
		sf::Vector2f m_anchor;
		sf::Vector2f m_pivot;
		sf::Vector2f m_localPosition;
		sf::Vector2f m_size = { 100.f, 100.f };

		bool m_sizeToContent = false;
	};
}