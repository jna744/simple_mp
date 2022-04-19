#ifndef SIMPLE_MP_ALGORITHMS_HPP
#define SIMPLE_MP_ALGORITHMS_HPP

#include <simple/mp/detail/config.hpp>

#include <simple/mp/bind.hpp>
#include <simple/mp/list.hpp>
#include <simple/mp/numeric_constants.hpp>
#include <simple/mp/utility.hpp>

#include <utility>

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

}  // namespace detail

template <typename L, template <typename...> class C>
using m_find_if = m_t_<detail::m_find_impl<m_size<L>::value, 0, C, m_assign<m_list<>, L>>>;

template <typename L, typename Q>
using m_find_if_q = m_find_if<L, Q::template invoke>;

template <typename L, typename T>
using m_find = m_find_if_q<L, m_bind_front<m_same, T>>;

namespace detail
{

template <std::size_t N, template <typename...> class C, typename L>
struct m_count_impl;

template <std::size_t N, template <typename...> class C, template <typename...> class L, typename T, typename... Ts>
struct m_count_impl<N, C, L<T, Ts...>>
  : m_if<m_to_bool<C<T>>, m_count_impl<N + 1, C, L<Ts...>>, m_count_impl<N, C, L<Ts...>>> {
};

template <std::size_t N, template <typename...> class C, template <typename...> class L>
struct m_count_impl<N, C, L<>> : m_size_t<N> {
};

}  // namespace detail

template <typename L, template <typename...> class C>
using m_count_if = m_t_<detail::m_count_impl<0, C, m_assign<m_list<>, L>>>;

template <typename L, typename Q>
using m_count_if_q = m_count_if<L, Q::template invoke>;

template <typename L, typename V>
using m_count = m_count_if_q<L, m_bind_front<m_same, V>>;

template <typename L, typename V>
using m_contains = m_not<m_same<m_find<L, V>, m_size<L>>>;

namespace detail
{

template <typename, typename>
struct m_transform_args;

template <typename... Ls, std::size_t I>
struct m_transform_args<m_list<Ls...>, m_size_t<I>> {
  using type = m_list<m_at_c<Ls, I>...>;
};

template <typename... Ls>
struct m_transform_args<m_list<Ls...>, m_size_t<0>> {
  using type = m_list<m_first<Ls>...>;
};

template <typename... Ls>
struct m_transform_args<m_list<Ls...>, m_size_t<1>> {
  using type = m_list<m_second<Ls>...>;
};

template <typename... Ls>
struct m_transform_args<m_list<Ls...>, m_size_t<2>> {
  using type = m_list<m_third<Ls>...>;
};

template <typename, template <typename...> class Fn, typename... Ls>
struct m_transform_impl;

template <std::size_t... Is, template <typename...> class Fn, typename... Ls>
struct m_transform_impl<std::index_sequence<Is...>, Fn, Ls...> {
  using type = m_list<m_apply<Fn, m_t_<m_transform_args<m_list<Ls...>, m_size_t<Is>>>>...>;
};

template <typename, template <typename...> class C, template <typename...> class Fn, typename... Ls>
struct m_transform_if_impl;

template <std::size_t... Is, template <typename...> class C, template <typename...> class Fn, typename... Ls>
struct m_transform_if_impl<std::index_sequence<Is...>, C, Fn, Ls...> {

  template <std::size_t I>
  using arg_list = m_t_<m_transform_args<m_list<Ls...>, m_size_t<I>>>;

  template <typename I>
  using t_yes = m_apply<Fn, arg_list<I::value>>;

  template <typename I>
  using t_no = m_at<m_first<m_list<Ls...>>, I>;

  using type = m_list<
      m_t_<m_if<m_to_bool<m_apply<C, arg_list<Is>>>, m_defer<t_yes, m_size_t<Is>>, m_defer<t_no, m_size_t<Is>>>>...>;
};

}  // namespace detail

template <template <typename...> class Fn, typename L, typename... Ls>
using m_transform =
    m_assign<L, m_t_<detail::m_transform_impl<std::make_index_sequence<m_size<L>::value>, Fn, L, Ls...>>>;

template <typename Q, typename L, typename... Ls>
using m_transform_q = m_transform<Q::template invoke, L, Ls...>;

template <template <typename...> class C, template <typename...> class Fn, typename L, typename... Ls>
using m_transform_if =
    m_assign<L, m_t_<detail::m_transform_if_impl<std::make_index_sequence<m_size<L>::value>, C, Fn, L, Ls...>>>;

template <typename Qc, typename Qt, typename L, typename... Ls>
using m_transform_if_q = m_transform_if<Qc::template invoke, Qt::template invoke, L, Ls...>;

namespace detail
{

template <typename L, typename U>
struct m_is_unique_impl {
  using type = m_true;
};

template <typename T, typename... Ts, typename U>
struct m_is_unique_impl<m_list<T, Ts...>, U> : m_if<
                                                   m_base_of<m_identity<T>, U>,
                                                   m_identity<m_false>,
                                                   m_is_unique_impl<m_list<Ts...>, m_push_back<U, m_identity<T>>>> {
};

template <typename L, typename U>
struct m_make_unique_impl {
  using type = m_assign<L, m_transform<m_t_, U>>;
};

template <typename T, typename... Ts, typename U>
struct m_make_unique_impl<m_list<T, Ts...>, U>
  : m_make_unique_impl<m_list<Ts...>, m_if<m_base_of<m_identity<T>, U>, U, m_push_back<U, m_identity<T>>>> {
};

}  // namespace detail

template <typename L>
using m_is_unique = m_t_<detail::m_is_unique_impl<L, m_inherit<>>>;

template <typename L>
using m_make_unique = m_t_<detail::m_make_unique_impl<L, m_inherit<>>>;

template <typename L, template <typename...> class P>
using m_all_of = m_bool<m_count_if<L, P>{} == m_size<L>{}>;

template <typename L, typename Qp>
using m_all_of_q = m_all_of<L, Qp::template invoke>;

template <typename L, template <typename...> class P>
using m_any_of = m_bool<(m_count_if<L, P>{} > 0)>;

template <typename L, typename Qp>
using m_any_of_q = m_any_of<L, Qp::template invoke>;

template <typename L, template <typename...> class P>
using m_none_of = m_not<m_any_of<L, P>>;

template <typename L, typename Qp>
using m_none_of_q = m_none_of<L, Qp::template invoke>;

namespace detail
{

template <typename>
struct m_from_seq_impl;

template <typename T, T... ts>
struct m_from_seq_impl<std::integer_sequence<T, ts...>> {
  using type = m_list_c<T, ts...>;
};

}  // namespace detail

template <typename Seq>
using m_from_seq = m_t_<detail::m_from_seq_impl<Seq>>;

}  // namespace mp

}  // namespace simple

#endif  // SIMPLE_MP_ALGORITHMS_HPP
