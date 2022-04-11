#ifndef SIMPLE_MP_INVOKE_WITH_INDEX_HPP
#define SIMPLE_MP_INVOKE_WITH_INDEX_HPP

#include <simple/mp/detail/config.hpp>

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

template <typename T>
using m_t_ = typename T::type;

template <typename>
struct m_fn_table_ptr_impl;

template <typename Return, typename... Args>
struct m_fn_table_ptr_impl<Return(Args...)> {
  using type = Return (*)(Args...);
};

template <typename T>
using m_fn_table_ptr = m_t_<m_fn_table_ptr_impl<T>>;

template <typename Signature, typename... Functions>
struct m_fn_table {
  static SMP_INLINE(17) constexpr m_fn_table_ptr<Signature> table[]{{&Functions::invoke}...};
};

#if SMP_CPP(14)
template <typename Signature, typename... Functions>
constexpr m_fn_table_ptr<Signature> m_fn_table<Signature, Functions...>::table[];
#endif

template <std::size_t I, typename Function, typename Return, typename... Args>
struct m_table_fn_wrap {
  static constexpr Return invoke(Function function, Args... args)
  {
    return std::forward<Function>(function)(m_size_t<I>{}, std::forward<Args>(args)...);
  }
};

template <typename Function, typename... Args, std::size_t... Is>
inline constexpr auto& m_construct_fn_table(std::index_sequence<Is...>)
{
  using fn_ret_type = decltype(m_declval<Function>()(m_size_t<0>{}, m_declval<Args>()...));
  using fns = m_list<m_table_fn_wrap<Is, Function, fn_ret_type, Args...>...>;
  using table_container = m_apply<m_fn_table, m_push_front<fns, fn_ret_type(Function, Args...)>>;
  return table_container::table;
}

}  // namespace detail

template <std::size_t Size, typename Function, typename... Args>
inline constexpr decltype(auto) m_invoke_with_index(std::size_t index, Function&& function, Args&&... args)
{
  SMP_ASSERT(index < Size);
  constexpr auto& table = detail::m_construct_fn_table<Function&&, Args&&...>(std::make_index_sequence<Size>{});
  return table[index](std::forward<Function>(function), std::forward<Args>(args)...);
}

}  // namespace mp

}  // namespace simple

#endif  // SIMPLE_MP_INVOKE_WITH_INDEX_HPP
