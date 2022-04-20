#ifndef SIMPLE_MP_BIND_HPP
#define SIMPLE_MP_BIND_HPP

#include <simple/mp/detail/config.hpp>
#include <simple/mp/detail/m_at_impl.hpp>

#include <simple/mp/list.hpp>
#include <simple/mp/numeric_constants.hpp>
#include <simple/mp/utility.hpp>

namespace simple
{

namespace mp
{

template <typename T>
using m_t_ = typename T::type;

template <std::size_t N>
struct m_arg {
  template <typename L>
  using type = m_t_<detail::m_at_impl<m_size_t<N>, L>>;
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
  using invoke = m_apply<Fn, m_list<detail::m_eval_bind_arg<Args, m_list<Us...>>...>>;
};

template <typename Q, typename... Args>
using m_bind_q = m_bind<Q::template invoke, Args...>;

template <template <typename...> class Fn, typename... Args>
struct m_bind_front {
  template <typename... Us>
  using invoke = m_apply<Fn, m_list<Args..., Us...>>;
};

template <typename Q, typename... Args>
using m_bind_front_q = m_bind_front<Q::template invoke, Args...>;

template <template <typename...> class Fn, typename... Args>
struct m_bind_back {
  template <typename... Us>
  using invoke = m_apply<Fn, m_list<Us..., Args...>>;
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

}  // namespace mp

}  // namespace simple

#endif  // SIMPLE_MP_BIND_HPP
