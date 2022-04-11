#ifndef SIMPLE_MP_UTILITY_HPP
#define SIMPLE_MP_UTILITY_HPP

#include <simple/mp/detail/config.hpp>

#include <simple/mp/list.hpp>
#include <simple/mp/numeric_constants.hpp>

#include <type_traits>

namespace simple
{

namespace mp
{

template <typename Base, typename Derived>
using m_base_of = std::is_base_of<Base, Derived>;

template <typename T>
using m_remove_cvref = std::remove_cv_t<std::remove_reference_t<T>>;

namespace detail
{

template <typename...>
struct m_void_impl {
  using type = void;
};

}  // namespace detail

template <typename... Ts>
using m_void = m_t_<detail::m_void_impl<Ts...>>;

template <typename T>
struct m_identity {
  using type = T;
};

template <typename T>
using m_identity_t = m_t_<m_identity<T>>;

template <typename... Ts>
struct m_inherit : Ts... {
};

template <typename T>
auto m_declval() noexcept -> std::add_rvalue_reference_t<T>;

template <std::size_t I>
struct m_tag_c : m_tag_c<I - 1> {
};

template <>
struct m_tag_c<0> {
};

template <typename I>
struct m_tag : m_tag<m_size_t<I::value - 1>> {
};

template <>
struct m_tag<m_size_t<0>> {
};

namespace detail
{

template <
    typename,  // always void
    template <typename...>
    class F,
    typename... Args>
struct m_is_valid_impl {
  using type = m_false;
};

template <template <typename...> class F, typename... Args>
struct m_is_valid_impl<m_void<F<Args...>>, F, Args...> {
  using type = m_true;
};

template <template <typename...> class Fn>
using m_detect_quote_t = void;

template <
    typename = void,  // always void
    typename...>
struct m_is_valid_q_impl {
  using type = m_false;
};

template <typename Q, typename... Args>
struct m_is_valid_q_impl<m_detect_quote_t<Q::template invoke>, Q, Args...> {
  using type = m_t_<m_is_valid_impl<void, Q::template invoke, Args...>>;
};

}  // namespace detail

template <template <typename...> class Fn, typename... Args>
using m_is_valid = m_t_<detail::m_is_valid_impl<void, Fn, Args...>>;

template <typename Q, typename... Args>
using m_is_valid_q = m_t_<detail::m_is_valid_q_impl<void, Q, Args...>>;

template <template <typename...> class Fn>
struct m_quote {
  template <typename... Args>
  using invoke = m_apply<Fn, m_list<Args...>>;
};

template <template <typename...> class Fn>
struct m_quote_trait {
  template <typename... Args>
  using invoke = m_t_<m_apply<Fn, m_list<Args...>>>;
};

template <typename Q, typename... Args>
using m_invoke_q = m_apply<Q::template invoke, m_list<Args...>>;

namespace detail
{

template <bool C, typename IfTrue, typename... IfFalse>
struct m_if_c_impl {
};

template <typename IfTrue, typename... IfFalse>
struct m_if_c_impl<true, IfTrue, IfFalse...> {
  using type = IfTrue;
};

template <typename IfTrue, typename IfFalse>
struct m_if_c_impl<false, IfTrue, IfFalse> {
  using type = IfFalse;
};

template <bool C, typename IfTrue, typename IfFalseQ, typename... Args>
struct m_eval_unless_impl {
  using type = m_invoke_q<IfFalseQ, Args...>;
};

template <typename IfTrue, typename IfFalseQ, typename... Args>
struct m_eval_unless_impl<true, IfTrue, IfFalseQ, Args...> {
  using type = IfTrue;
};

}  // namespace detail

template <bool C, typename IfTrue, typename... IfFalse>
using m_if_c = m_t_<detail::m_if_c_impl<C, IfTrue, IfFalse...>>;

template <typename C, typename IfTrue, typename... IfFalse>
using m_if = m_if_c<bool(C::value), IfTrue, IfFalse...>;

template <bool C, typename T, template <typename...> class Fn, typename... Args>
using m_eval_unless_c = m_t_<detail::m_eval_unless_impl<C, T, m_quote<Fn>, Args...>>;

template <bool C, typename T, typename Q, typename... Args>
using m_eval_unless_c_q = m_t_<detail::m_eval_unless_impl<C, T, Q, Args...>>;

template <typename C, typename T, template <typename...> class Fn, typename... Args>
using m_eval_unless = m_eval_unless_c<bool(C::value), T, Fn, Args...>;

template <typename C, typename T, typename Q, typename... Args>
using m_eval_unless_q = m_t_<detail::m_eval_unless_impl<bool(C::value), T, Q, Args...>>;

template <typename C, typename T, template <typename...> class Fn, typename... Args>
using m_eval_if = m_eval_unless<m_not<C>, T, Fn, Args...>;

template <typename C, typename T, typename Q, typename... Args>
using m_eval_if_q = m_eval_unless_q<m_not<C>, T, Q, Args...>;

template <typename T, template <typename...> class Fn, typename... Args>
using m_eval_or = m_eval_if<m_is_valid<Fn, Args...>, T, Fn, Args...>;

template <typename T, typename Q, typename... Args>
using m_eval_or_q = m_eval_if_q<m_is_valid_q<Q, Args...>, T, Q, Args...>;

template <template <typename...> class Fn, typename... Args>
struct m_defer {
  using type = Fn<Args...>;
};

template <typename Q, typename... Args>
struct m_defer_q {
  using type = m_invoke_q<Q, Args...>;
};

template <template <typename...> class Fn>
struct m_not_fn {
  template <typename... Args>
  using invoke = m_not<Fn<Args...>>;
};

template <typename Q>
using m_not_fn_q = m_not_fn<Q::template invoke>;

namespace detail
{

template <template <typename...> class F1, template <typename...> class F2>
struct m_composed_impl {
  template <typename... Ts>
  using invoke = m_apply<F2, m_list<m_apply<F1, m_list<Ts...>>>>;
};

template <template <typename...> class... Functions>
struct m_compose_impl;

template <template <typename...> class F1, template <typename...> class F2, template <typename...> class... Functions>
struct m_compose_impl<F1, F2, Functions...> : m_compose_impl<m_composed_impl<F1, F2>::template invoke, Functions...> {
};

template <template <typename...> class Function>
struct m_compose_impl<Function> : m_quote<Function> {
};

}  // namespace detail

template <template <typename...> class... Functions>
using m_compose = detail::m_compose_impl<Functions...>;

namespace detail
{

template <typename... Ts>
struct m_all_impl {
  using type = m_true;
};

template <typename T, typename... Ts>
struct m_all_impl<T, Ts...> {
  using type = m_if<m_eval_or<m_false, m_to_bool, T>, m_t_<m_all_impl<Ts...>>, m_false>;
};

template <typename... Ts>
struct m_any_impl {
  using type = m_false;
};

template <typename T, typename... Ts>
struct m_any_impl<T, Ts...> {
  using type = m_if<m_eval_or<m_false, m_to_bool, T>, m_true, m_t_<m_any_impl<Ts...>>>;
};

}  // namespace detail

template <typename... Ts>
using m_all = m_t_<detail::m_all_impl<Ts...>>;

template <typename... Ts>
using m_any = m_t_<detail::m_any_impl<Ts...>>;

namespace detail
{

template <typename... Ts>
struct m_same_impl : m_true {
};

template <typename T, typename... Ts>
struct m_same_impl<T, T, Ts...> : m_same_impl<T, Ts...> {
};

template <typename T, typename... Ts>
struct m_same_impl<T, T, T, Ts...> : m_same_impl<T, Ts...> {
};

template <typename T, typename... Ts>
struct m_same_impl<T, T, T, T, Ts...> : m_same_impl<T, Ts...> {
};

template <typename T, typename... Ts>
struct m_same_impl<T, T, T, T, T, Ts...> : m_same_impl<T, Ts...> {
};

template <typename T, typename... Ts>
struct m_same_impl<T, T, T, T, T, T, Ts...> : m_same_impl<T, Ts...> {
};

template <typename T, typename U, typename... Ts>
struct m_same_impl<T, U, Ts...> : m_false {
};

template <typename T, typename U>
using m_min_type = m_if_c<(T::value < U::value), T, U>;

template <typename T, typename U>
using m_max_type = m_if_c<(T::value > U::value), T, U>;

template <template <typename...> class Fn, typename... Ts>
struct m_size_test {
};

template <template <typename...> class Fn, typename T, typename U, typename... Ts>
struct m_size_test<Fn, T, U, Ts...> : m_size_test<Fn, Fn<T, U>, Ts...> {
};

template <template <typename...> class Fn, typename T>
struct m_size_test<Fn, T> {
  using type = T;
};

}  // namespace detail

template <typename... Ts>
using m_same = detail::m_same_impl<Ts...>;

template <typename... Ts>
using m_max = m_t_<detail::m_size_test<detail::m_max_type, Ts...>>;

template <std::size_t... Is>
using m_max_c = m_max<m_size_t<Is>...>;

template <typename... Ts>
using m_min = m_t_<detail::m_size_test<detail::m_min_type, Ts...>>;

template <std::size_t... Is>
using m_min_c = m_min<m_size_t<Is>...>;
}  // namespace mp

}  // namespace simple

#endif  // SIMPLE_MP_UTILITY_HPP
