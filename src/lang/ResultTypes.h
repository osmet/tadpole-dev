#pragma once

#include <tl/expected.hpp>

namespace lang
{
    template<typename TValue, typename TError>
    using Expected = tl::expected<TValue, TError>;

    template<typename TValue, typename TError>
    using ExpectedRef = tl::expected<std::reference_wrapper<TValue>, TError>;

    template<typename TValue, typename TError>
    using ExpectedConstRef = tl::expected<std::reference_wrapper<const TValue>, TError>;

    template<typename TValue>
    using Optional = std::optional<TValue>;

    template<typename TValue>
    using OptionalRef = std::optional<std::reference_wrapper<TValue>>;

    template<typename TValue>
    using OptionalConstRef = std::optional<std::reference_wrapper<const TValue>>;
}