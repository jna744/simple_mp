#ifndef SIMPLE_MP_DETAIL_M_AT_IMPL_HPP
#define SIMPLE_MP_DETAIL_M_AT_IMPL_HPP

#include <simple/mp/detail/config.hpp>
#include <simple/mp/numeric_constants.hpp>

namespace simple
{

namespace mp
{

namespace detail
{

template <typename, typename>
struct m_at_impl;

template <std::size_t I, template <typename...> class L, typename T, typename... Ts>
struct m_at_impl<m_size_t<I>, L<T, Ts...>> : m_at_impl<m_size_t<I - 1>, L<Ts...>> {
};

template <typename T, template <typename...> class L, typename... Ts>
struct m_at_impl<m_size_t<0>, L<T, Ts...>> {
  using type = T;
};

}  // namespace detail

}  // namespace mp

}  // namespace simple

#endif  // SIMPLE_MP_DETAIL_M_AT_IMPL_HPP
