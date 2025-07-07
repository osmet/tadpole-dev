#pragma once

namespace lang
{
    template<typename TValue>
    class ReactiveProperty
    {
    public:
        using OnValueChange = std::function<void(const TValue&)>;

        ReactiveProperty() = default;

        ReactiveProperty(const TValue& initialValue)
            : m_value(initialValue)
        {
        }

        TValue GetValue() const
        {
            return m_value;
        }

        void SetValue(const TValue& newValue)
        {
            m_value = newValue;

            for (auto& subscriber : m_subscribers)
            {
                subscriber(newValue);
            }
        }

        void SetValue(const TValue&& newValue)
        {
            m_value = std::move(newValue);

            for (auto& subscriber : m_subscribers)
            {
                subscriber(newValue);
            }
        }

        void Subscribe(OnValueChange callback)
        {
            m_subscribers.emplace_back(std::move(callback));
        }

    private:
        TValue m_value{};
        std::vector<OnValueChange> m_subscribers;
    };
}
