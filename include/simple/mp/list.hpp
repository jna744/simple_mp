#ifndef SIMPLE_MP_LIST_HPP
#define SIMPLE_MP_LIST_HPP

#include <simple/mp/detail/config.hpp>

#include <simple/mp/numeric_constants.hpp>

#include <type_traits>

namespace simple
{

namespace mp
{

template <typename T>
using m_t_ = typename T::type;

template <typename... Ts>
struct m_list {
  using type = m_list;
  static constexpr std::size_t size = sizeof...(Ts);
};

template <typename T, T... ts>
using m_list_c = m_list<m_integral<T, ts>...>;

template <typename L>
struct m_size : m_size_t<0> {
};

template <template <typename...> class L, typename... Ts>
struct m_size<L<Ts...>> : m_size_t<sizeof...(Ts)> {
};

template <typename L>
using m_empty = m_bool<m_size<L>{} == 0>;

namespace detail
{

template <typename, typename>
struct m_assign_impl;

template <template <typename...> class L1, template <typename...> class L2, typename... Ts, typename... Us>
struct m_assign_impl<L1<Ts...>, L2<Us...>> {
  using type = L1<Us...>;
};

}  // namespace detail

template <typename L1, typename L2>
using m_assign = m_t_<detail::m_assign_impl<L1, L2>>;

template <typename L>
using m_clear = m_assign<L, m_list<>>;

namespace detail
{

template <typename L>
struct m_first_impl {
};

template <template <typename...> class L, typename T, typename... Ts>
struct m_first_impl<L<T, Ts...>> {
  using type = T;
};

template <typename L>
struct m_second_impl {
};

template <template <typename...> class L, typename T1, typename T2, typename... Ts>
struct m_second_impl<L<T1, T2, Ts...>> {
  using type = T2;
};

template <typename L>
struct m_third_impl {
};

template <template <typename...> class L, typename T1, typename T2, typename T3, typename... Ts>
struct m_third_impl<L<T1, T2, T3, Ts...>> {
  using type = T3;
};

};  // namespace detail

template <typename L>
using m_first = m_t_<detail::m_first_impl<L>>;

template <typename L>
using m_second = m_t_<detail::m_second_impl<L>>;

template <typename L>
using m_third = m_t_<detail::m_third_impl<L>>;

namespace detail
{

template <typename C, typename L, typename... Ts>
struct m_push_impl;

template <typename PushFrontC, template <typename...> class L, typename... Us, typename... Ts>
// Push front
struct m_push_impl<PushFrontC, L<Us...>, Ts...> {
  using type = L<Ts..., Us...>;
};

template <template <typename...> class L, typename... Us, typename... Ts>
// Push back
struct m_push_impl<m_false, L<Us...>, Ts...> {
  using type = L<Us..., Ts...>;
};

template <typename L>
struct m_pop_front_impl;

template <template <typename...> class L, typename T, typename... Ts>
struct m_pop_front_impl<L<T, Ts...>> {
  using type = L<Ts...>;
};

}  // namespace detail

template <typename L, typename... Ts>
using m_push_back = m_t_<detail::m_push_impl<m_false, L, Ts...>>;

template <typename L, typename... Ts>
using m_push_front = m_t_<detail::m_push_impl<m_true, L, Ts...>>;

template <typename L>
using m_pop_front = m_t_<detail::m_pop_front_impl<L>>;

namespace detail
{

template <typename, template <typename...> class>
struct m_rename_impl;

template <template <typename...> class L, typename... Ts, template <typename...> class F>
struct m_rename_impl<L<Ts...>, F> {
  using type = F<Ts...>;
};

}  // namespace detail

template <typename L, template <typename...> class Fn>
using m_rename = m_t_<detail::m_rename_impl<L, Fn>>;

template <template <typename...> class Fn, typename L>
using m_apply = m_rename<L, Fn>;

template <typename Q, typename L>
using m_apply_q = m_apply<Q::template invoke, L>;

namespace detail
{

template <typename...>
struct m_append_impl {
  using type = m_list<>;
};

template <
    template <typename...>
    class L1,
    template <typename...>
    class L2,
    typename... T1s,
    typename... T2s,
    typename... Ls>
struct m_append_impl<L1<T1s...>, L2<T2s...>, Ls...> : m_append_impl<L1<T1s..., T2s...>, Ls...> {
};

template <
    template <typename...>
    class L1,
    template <typename...>
    class L2,
    template <typename...>
    class L3,
    typename... T1s,
    typename... T2s,
    typename... T3s,
    typename... Ls>
struct m_append_impl<L1<T1s...>, L2<T2s...>, L3<T3s...>, Ls...> : m_append_impl<L1<T1s..., T2s..., T3s...>, Ls...> {
};

template <
    template <typename...>
    class L1,
    template <typename...>
    class L2,
    template <typename...>
    class L3,
    template <typename...>
    class L4,
    typename... T1s,
    typename... T2s,
    typename... T3s,
    typename... T4s,
    typename... Ls>
struct m_append_impl<L1<T1s...>, L2<T2s...>, L3<T3s...>, L4<T4s...>, Ls...>
  : m_append_impl<L1<T1s..., T2s..., T3s..., T4s...>, Ls...> {
};

template <
    template <typename...>
    class L1,
    template <typename...>
    class L2,
    template <typename...>
    class L3,
    template <typename...>
    class L4,
    template <typename...>
    class L5,
    typename... T1s,
    typename... T2s,
    typename... T3s,
    typename... T4s,
    typename... T5s,
    typename... Ls>
struct m_append_impl<L1<T1s...>, L2<T2s...>, L3<T3s...>, L4<T4s...>, L5<T5s...>, Ls...>
  : m_append_impl<L1<T1s..., T2s..., T3s..., T4s..., T5s...>, Ls...> {
};

template <typename Result>
struct m_append_impl<Result> {
  using type = Result;
};

}  // namespace detail

template <typename... Ls>
using m_append = m_t_<detail::m_append_impl<Ls...>>;

namespace detail
{

template <template <typename...> class F, typename I, typename L>
struct m_transform_index;

template <template <typename...> class F, template <typename...> class L, typename T1, typename... Ts>
struct m_transform_index<F, m_size_t<0>, L<T1, Ts...>> {
  using type = L<F<T1>, Ts...>;
};

template <template <typename...> class F, template <typename...> class L, typename T1, typename T2, typename... Ts>
struct m_transform_index<F, m_size_t<1>, L<T1, T2, Ts...>> {
  using type = L<T1, F<T2>, Ts...>;
};

template <
    template <typename...>
    class F,
    template <typename...>
    class L,
    typename T1,
    typename T2,
    typename T3,
    typename... Ts>
struct m_transform_index<F, m_size_t<2>, L<T1, T2, T3, Ts...>> {
  using type = L<T1, T2, F<T3>, Ts...>;
};

template <typename T>
struct m_replace_fn {
  template <typename... Args>
  using invoke = T;
};

}  // namespace detail

template <typename L, typename T>
using m_replace_first = m_t_<detail::m_transform_index<detail::m_replace_fn<T>::template invoke, m_size_t<0>, L>>;

template <typename L, typename T>
using m_replace_second = m_t_<detail::m_transform_index<detail::m_replace_fn<T>::template invoke, m_size_t<1>, L>>;

template <typename L, typename T>
using m_replace_third = m_t_<detail::m_transform_index<detail::m_replace_fn<T>::template invoke, m_size_t<2>, L>>;

template <typename L, template <typename...> class F>
using m_transform_first = m_t_<detail::m_transform_index<F, m_size_t<0>, L>>;

template <typename L, typename Q>
using m_transform_first_q = m_transform_first<L, Q::template invoke>;

template <typename L, template <typename...> class F>
using m_transform_second = m_t_<detail::m_transform_index<F, m_size_t<1>, L>>;

template <typename L, typename Q>
using m_transform_second_q = m_transform_second<L, Q::template invoke>;

template <typename L, template <typename...> class F>
using m_transform_third = m_t_<detail::m_transform_index<F, m_size_t<2>, L>>;

template <typename L, typename Q>
using m_transform_third_q = m_transform_third<L, Q::template invoke>;

}  // namespace mp

}  // namespace simple

#endif  // SIMPLE_MP_LIST_HPP
