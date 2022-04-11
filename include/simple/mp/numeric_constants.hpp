#ifndef SIMPLE_MP_NUMERIC_TYPES_HPP
#define SIMPLE_MP_NUMERIC_TYPES_HPP

#include <simple/mp/detail/config.hpp>

#include <type_traits>

namespace simple
{

namespace mp
{

template <typename T, T V>
using m_integral = std::integral_constant<T, V>;

template <std::size_t V>
using m_size_t = m_integral<std::size_t, V>;

template <bool V>
using m_bool = m_integral<bool, V>;

using m_true = m_bool<true>;

using m_false = m_bool<false>;

template <typename T>
using m_to_bool = m_bool<bool(T::value)>;

template <typename T>
using m_not = m_bool<!bool(T::value)>;

}  // namespace mp

}  // namespace simple

#endif  // SIMPLE_MP_FWD_HPP
