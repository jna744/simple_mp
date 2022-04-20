#ifndef SIMPLE_MP_DETAIL_M_INDEXED_VTABLE_INVOKE_HPP
#define SIMPLE_MP_DETAIL_M_INDEXED_VTABLE_INVOKE_HPP

#include <simple/mp/bind.hpp>
#include <simple/mp/detail/config.hpp>
#include <simple/mp/numeric_constants.hpp>

#include <type_traits>
#include <utility>

namespace simple
{

namespace mp
{

namespace detail
{

template <typename>
struct table_ptr_impl;

template <typename Return, typename... Args>
struct table_ptr_impl<Return(Args...)> {
  using type = Return (*)(Args...);
};

template <typename T>
using table_ptr = typename table_ptr_impl<T>::type;

template <typename Signature, typename... Functions>
struct fn_table {
  static SMP_INLINE(17) constexpr table_ptr<Signature> table[]{{&Functions::invoke}...};
};

#if SMP_CPP(14)
template <typename Signature, typename... Functions>
constexpr table_ptr<Signature> fn_table<Signature, Functions...>::table[];
#endif

template <typename I, typename Function, typename Return, typename... Args>
struct table_function {
  static constexpr Return invoke(Function function, Args... args)
  {
    return std::forward<Function>(function)(I{}, std::forward<Args>(args)...);
  }
};

}  // namespace detail

template <std::size_t Size, typename Function, typename... Args>
[[deprecated]] inline constexpr decltype(auto)
m_indexed_vtable_invoke_c(std::size_t index, Function&& function, Args&&... args)
{
  SMP_ASSERT(index < Size);

  using ret_t = decltype(std::declval<Function>()(m_size_t<0>{}, std::declval<Args>()...));

  using table_fn = m_bind_back<detail::table_function, Function, ret_t, Args...>;

  using table_fns = m_transform_q<table_fn, m_from_seq<std::make_index_sequence<Size>>>;

  constexpr auto& table = m_apply<detail::fn_table, m_push_front<table_fns, ret_t(Function, Args...)>>::table;

  return table[index](std::forward<Function>(function), std::forward<Args>(args)...);
}

template <typename S, typename Function, typename... Args>
[[deprecated]] inline constexpr decltype(auto)
m_indexed_vtable_invoke(std::size_t index, Function&& function, Args&&... args)
{
  return m_indexed_vtable_invoke_c<std::size_t(S::value)>(
      index, std::forward<Function>(function), std::forward<Args>(args)...);
}

}  // namespace mp

}  // namespace simple

#endif  // SIMPLE_MP_DETAIL_M_INDEXED_VTABLE_INVOKE_HPP
