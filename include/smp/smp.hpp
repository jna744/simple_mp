#ifndef SMP_HPP
#define SMP_HPP

#include <type_traits>
#include <utility>

namespace smp
{

template <typename T>
using m_t_ = typename T::type;

// Numeric types

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

// List operations begin -----------------------------------------------------------------

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

// List operations end -------------------------------------------------------------------

// Utility begin -------------------------------------------------------------------------

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
auto m_declval() -> std::add_rvalue_reference_t<T>;

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

// Utility end ---------------------------------------------------------------------------

// Helper functions begin ----------------------------------------------------------------

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

// Helper functions end  -----------------------------------------------------------------

// Algorithms begin ----------------------------------------------------------------------

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

template <std::size_t, typename, typename, typename = void>
struct m_take_impl;

template <std::size_t N, typename T, typename... Ts, typename... Us>
struct m_take_impl<N, m_list<T, Ts...>, m_list<Us...>, m_if_c<N != 0, void>>
  : m_take_impl<N - 1, m_list<Ts...>, m_list<Us..., T>> {
};

template <typename Rest, typename Result, typename Enable>
struct m_take_impl<0, Rest, Result, Enable> {
  using result = Result;
  using rest = Rest;
};

}  // namespace detail

template <typename L, typename I>
using m_at = m_t_<detail::m_at_impl<I, L>>;

template <typename L, std::size_t I>
using m_at_c = m_t_<detail::m_at_impl<m_size_t<I>, L>>;

template <typename L, std::size_t N>
using m_take_c = m_assign<L, typename detail::m_take_impl<N, m_apply<m_list, L>, m_list<>>::result>;

template <typename L, typename N>
using m_take = m_take_c<L, std::size_t(N::value)>;

template <typename L>
using m_pop_back = m_take_c<L, m_size<L>::value - 1>;

template <typename L>
using m_last = m_at_c<L, m_size<L>::value - 1>;

namespace detail
{

template <std::size_t Fallback, std::size_t I, template <typename...> class If, typename L>
struct m_find_impl {
  using type = m_size_t<Fallback>;
};

template <
    std::size_t Fallback,
    std::size_t I,
    template <typename...>
    class If,
    template <typename...>
    class L,
    typename U,
    typename... Ts>
struct m_find_impl<Fallback, I, If, L<U, Ts...>>
  : m_if<If<U>, m_identity<m_size_t<I>>, m_find_impl<Fallback, I + 1, If, L<Ts...>>> {
};

template <typename T>
struct m_is_same_as {
  template <typename U, typename...>
  using invoke = m_same<T, U>;
};

}  // namespace detail

template <typename L, template <typename...> class C>
using m_find_if = m_t_<detail::m_find_impl<m_size<L>::value, 0, C, m_assign<m_list<>, L>>>;

template <typename L, typename Q>
using m_find_if_q = m_find_if<L, Q::template invoke>;

template <typename L, typename T>
using m_find = m_find_if_q<L, detail::m_is_same_as<T>>;

// Algorithms end ------------------------------------------------------------------------

// Bind begin ----------------------------------------------------------------------------

template <std::size_t N>
struct m_arg {
  template <typename L>
  using type = m_at_c<L, N>;
};

template <template <typename...> class Fn, typename... Args>
struct m_bind;

namespace detail
{

template <typename>
struct m_is_arg : m_false {
};

template <std::size_t I>
struct m_is_arg<m_arg<I>> : m_true {
};

template <typename>
struct m_is_bound : m_false {
};

template <template <typename...> class Fn, typename... Args>
struct m_is_bound<m_bind<Fn, Args...>> : m_true {
};

template <typename Arg, typename L, typename = void>
struct m_eval_bind_arg_impl {
  using type = Arg;
};

template <typename Arg, typename L>
struct m_eval_bind_arg_impl<Arg, L, m_if<m_is_arg<Arg>, void>> {
  using type = typename Arg::template type<L>;
};

template <typename Arg, typename L>
struct m_eval_bind_arg_impl<Arg, L, m_if<m_is_bound<Arg>, void>> {
  using type = m_apply<m_invoke_q, m_push_front<L, Arg>>;
};

template <typename Arg, typename L>
using m_eval_bind_arg = typename m_eval_bind_arg_impl<Arg, L>::type;

}  // namespace detail

template <template <typename...> class Fn, typename... Args>
struct m_bind {
  template <typename... Us>
  using invoke = Fn<detail::m_eval_bind_arg<Args, m_list<Us...>>...>;
};

template <typename Q, typename... Args>
using m_bind_q = m_bind<Q::template invoke, Args...>;

template <template <typename...> class Fn, typename... Args>
struct m_bind_front {
  template <typename... Us>
  using invoke = Fn<Args..., Us...>;
};

template <typename Q, typename... Args>
using m_bind_front_q = m_bind_front<Q::template invoke, Args...>;

template <template <typename...> class Fn, typename... Args>
struct m_bind_back {
  template <typename... Us>
  using invoke = Fn<Us..., Args...>;
};

template <typename Q, typename... Args>
using m_bind_back_q = m_bind_back<Q::template invoke, Args...>;

inline namespace placeholders
{

using _1 = m_arg<0>;
using _2 = m_arg<1>;
using _3 = m_arg<2>;
using _4 = m_arg<3>;
using _5 = m_arg<4>;
using _6 = m_arg<5>;

}  // namespace placeholders

// Bind end ------------------------------------------------------------------------------

// Map begin -----------------------------------------------------------------------------

namespace detail
{

template <typename K, typename L>
struct m_map_key_value {
  using key = K;
  using value = L;
};

template <typename Base, typename Derived>
using m_base_of = std::is_base_of<Base, Derived>;

template <typename... Ts>
struct m_map_impl : m_inherit<m_identity<m_first<Ts>>...>, detail::m_map_key_value<m_first<Ts>, m_pop_front<Ts>>... {
  using keys = m_list<m_identity<m_first<Ts>>...>;
  using type = m_map_impl;
  static constexpr std::size_t size = sizeof...(Ts);
};

template <>
struct m_map_impl<> {
  using keys = m_list<>;
  using type = m_map_impl;
  static constexpr std::size_t size = 0;
};

template <typename Map, typename Key>
struct m_map_get_impl {
  template <typename K, typename V>
  static auto value(detail::m_map_key_value<K, V> const*) -> V;
  using type = decltype(value<Key>(static_cast<Map const*>(0)));
};

}  // namespace detail

template <typename>
struct m_is_map : m_false {
};

template <typename K, typename V>
struct m_is_map<detail::m_map_key_value<K, V>> : m_true {
};

template <typename... Ts>
using m_map = m_t_<detail::m_map_impl<Ts...>>;

template <typename Map, typename Key>
using m_map_contains = detail::m_base_of<m_identity<Key>, m_assign<m_inherit<>, typename Map::keys>>;

template <typename Map, typename Key, typename... Values>
using m_map_insert = m_eval_unless<m_map_contains<Map, Key>, Map, m_push_back, Map, m_list<Key, Values...>>;

template <typename Map, typename Key>
using m_map_get = m_t_<detail::m_map_get_impl<Map, Key>>;

// Map end -------------------------------------------------------------------------------

// Type name begin -----------------------------------------------------------------------

namespace detail
{

struct m_char_traits {

  using CharT = char;

  static constexpr int compare(CharT const* ch1, CharT const* ch2, std::size_t n) noexcept
  {
    for (std::size_t i = 0; i < n; ++i)
      if (ch1[i] < ch2[i])
        return -1;
      else if (ch2[i] < ch1[i])
        return 1;
    return 0;
  }

  static constexpr CharT const* find(CharT const* s, std::size_t n, CharT const& ch) noexcept
  {
    for (std::size_t i = 0; i < n; ++i)
      if (s[i] == ch)
        return s + i;
    return 0;
  }
};

// TODO: inline in cpp17
constexpr auto m_string_npos = std::size_t(-1);

struct m_string_exception {
};

template <std::size_t N, typename Traits = detail::m_char_traits>
struct m_string {

  using CharT = char;
  CharT const value[N + 1];

  template <std::size_t... Is>
  constexpr m_string(CharT const* ptr, std::index_sequence<Is...>) : value{ptr[Is]..., '\0'}
  {
  }

  constexpr m_string(CharT const* ptr) : m_string(ptr, std::make_index_sequence<N>{}) {}

  constexpr std::size_t find(CharT const* s, std::size_t pos, std::size_t count) const
  {
    if (!count)
      return pos <= N ? pos : m_string_npos;
    if (pos >= N)
      return m_string_npos;
    CharT const*       first = value + pos;
    CharT const* const last = value + N;
    for (auto remaining = N - pos; remaining >= count; remaining = last - ++first) {
      if (!(first = Traits::find(first, remaining - count + 1, s[0])))
        return m_string_npos;

      if (Traits::compare(first, s, count) == 0)
        return first - value;
    }
    return m_string_npos;
  }

  constexpr std::size_t rfind(CharT const* s, std::size_t pos, std::size_t count) const
  {
    if (count <= N) {
      auto temp = N - count;
      pos = pos < temp ? pos : temp;
      do {
        if (Traits::compare(value + pos, s, count) == 0)
          return pos;
      } while (pos-- > 0);
    }
    return m_string_npos;
  }

  template <std::size_t Position = 0, std::size_t Count = m_string_npos>
  constexpr m_string<m_min_c<N - Position, Count>::value> substr() const
  {
    return (
        Position > N ? throw m_string_exception{} : m_string<m_min_c<N - Position, Count>::value>{value + Position});
  }
};

template <typename T>
inline constexpr auto m_construct_type_name_impl()
{
#if defined(__clang__)
  constexpr auto& prefix = "[T = ";
  constexpr auto& suffix = "]";
  constexpr auto& function = __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
  constexpr auto& prefix = "with T = ";
  constexpr auto& suffix = "]";
  constexpr auto& function = __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
  constexpr auto& prefix = "type_name_array<";
  constexpr auto& suffix = ">(void)";
  constexpr auto& function = __FUNCSIG__;
#else
#error "smp::m_type_name<T>() not available on this compiler"
#endif

  constexpr auto str = m_string<sizeof(decltype(function)) - 1>(function);
  constexpr auto start = str.find(prefix, 0, sizeof(prefix) - 1) + sizeof(prefix) - 1;
  constexpr auto end = str.rfind(suffix, m_string_npos, sizeof(suffix) - 1);
  static_assert(start < end, "");
  return str.template substr<start, end - start>();
}

template <typename T>
struct m_construct_type_name {
  using type = decltype(m_construct_type_name_impl<T>());
  static constexpr type value = m_construct_type_name_impl<T>();
};

template <typename T>
constexpr typename m_construct_type_name<T>::type m_construct_type_name<T>::value;

}  // namespace detail

template <typename T>
inline constexpr char const* m_type_name() noexcept
{
  constexpr auto& name = detail::m_construct_type_name<T>::value;
  return name.value;
}

// Typename end --------------------------------------------------------------------------

// Function table begin ------------------------------------------------------------------

namespace detail
{

template <typename>
struct m_fn_type_impl;

template <typename Return, typename... Args>
struct m_fn_type_impl<Return(Args...)> {
  using type = Return (*)(Args...);
};

template <typename T>
using m_fn_type = m_t_<m_fn_type_impl<T>>;

template <typename Signature, typename... Functions>
struct m_fn_table {
  using fn_type = m_fn_type<Signature>;
  static constexpr fn_type table[]{{&Functions::invoke}...};
};

template <typename Signature, typename... Functions>
constexpr m_fn_type<Signature> m_fn_table<Signature, Functions...>::table[];

template <std::size_t I, typename Function, typename Return, typename... Args>
struct m_table_fn_wrap {
  static constexpr Return invoke(Function function, Args... args)
  {
    return Function{std::move(function)}(m_size_t<I>{}, std::forward<Args>(args)...);
  }
};

template <typename Function, typename... Args, std::size_t... Is>
constexpr auto m_construct_fn_table_impl(std::index_sequence<Is...>)
{
  using fn_ret_type = decltype(m_declval<Function>()(m_size_t<0>{}, m_declval<Args>()...));
  using fns = m_list<m_table_fn_wrap<Is, Function, fn_ret_type, Args...>...>;
  using table = m_apply<m_fn_table, m_push_front<fns, fn_ret_type(Function, Args...)>>;
  return table{};
}

template <std::size_t I, typename Function, typename... Args>
struct m_construct_fn_table {
  using table_type = decltype(m_construct_fn_table_impl<Function, Args...>(std::make_index_sequence<I>{}));
  static constexpr table_type value = m_construct_fn_table_impl<Function, Args...>(std::make_index_sequence<I>{});
};

}  // namespace detail

template <std::size_t Size, typename Function, typename... Args>
constexpr decltype(auto) m_table_invoke(std::size_t index, Function&& function, Args&&... args)
{
  constexpr auto& table = detail::m_construct_fn_table<Size, Function, Args&&...>::value.table;
  return table[index](std::forward<Function>(function), std::forward<Args>(args)...);
}

// Function table end --------------------------------------------------------------------

}  // namespace smp

#endif  // SMP_SMP_HPP
