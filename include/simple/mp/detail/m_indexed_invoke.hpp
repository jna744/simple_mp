#ifndef SIMPLE_MP_DETAIL_M_INDEXED_INVOKE_HPP
#define SIMPLE_MP_DETAIL_M_INDEXED_INVOKE_HPP

#include <simple/mp/detail/config.hpp>
#include <simple/mp/numeric_constants.hpp>

#include <tuple>
#include <utility>

#if defined __GNUC__
#define __SMP_UNREACHABLE__ __builtin_unreachable();
#else
#define __SMP_UNREACHABLE__ void(0);
#endif

namespace simple
{

namespace mp
{

template <std::size_t N, typename Function, typename... Args>
inline constexpr decltype(auto) m_indexed_invoke(std::size_t index, Function&& function, Args&&... args);

namespace detail
{

template <std::size_t N>
struct indexed_invoke {
  template <std::size_t Acc, typename Function, typename... Args>
  static constexpr decltype(auto) invoke(std::size_t index, Function&& function, Args&&... args)
  {
    if (index < (N / 2))
      return indexed_invoke<N / 2>::template invoke<Acc>(
          index, std::forward<Function>(function), std::forward<Args>(args)...);
    else
      return indexed_invoke<N - (N / 2)>::template invoke<Acc + (N / 2)>(
          index - (N / 2), std::forward<Function>(function), std::forward<Args>(args)...);
  }
};

template <>
struct indexed_invoke<1> {
  template <std::size_t Acc, typename Function, typename... Args>
  static constexpr decltype(auto) invoke(std::size_t index, Function&& function, Args&&... args)
  {
    return std::forward<Function>(function)(m_size_t<Acc + 0>{}, std::forward<Args>(args)...);
  }
};

#define __II_C__(n)                                                                                                    \
  case n:                                                                                                              \
    return std::forward<Function>(function)(m_size_t<Acc + n>{}, std::forward<Args>(args)...)

#define __II__(n, body)                                                                                                \
  template <>                                                                                                          \
  struct indexed_invoke<n> {                                                                                           \
    template <std::size_t Acc, typename Function, typename... Args>                                                    \
    static constexpr decltype(auto) invoke(std::size_t index, Function&& function, Args&&... args)                     \
    {                                                                                                                  \
      switch (index) {                                                                                                 \
        default: __SMP_UNREACHABLE__ body;                                                                             \
      }                                                                                                                \
    }                                                                                                                  \
  };

__II__(2, __II_C__(0); __II_C__(1))

__II__(3, __II_C__(0); __II_C__(1); __II_C__(2))

__II__(4, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3))

__II__(5, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4))

__II__(6, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5))

__II__(7, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5); __II_C__(6))

__II__(8, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5); __II_C__(6); __II_C__(7))

__II__(9, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5); __II_C__(6); __II_C__(7);
       __II_C__(8))

__II__(10, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5); __II_C__(6); __II_C__(7);
       __II_C__(8);
       __II_C__(9))

__II__(11, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5); __II_C__(6); __II_C__(7);
       __II_C__(8);
       __II_C__(9);
       __II_C__(10))

__II__(12, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5); __II_C__(6); __II_C__(7);
       __II_C__(8);
       __II_C__(9);
       __II_C__(10);
       __II_C__(11))

__II__(13, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5); __II_C__(6); __II_C__(7);
       __II_C__(8);
       __II_C__(9);
       __II_C__(10);
       __II_C__(11);
       __II_C__(12))

__II__(14, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5); __II_C__(6); __II_C__(7);
       __II_C__(8);
       __II_C__(9);
       __II_C__(10);
       __II_C__(11);
       __II_C__(12);
       __II_C__(13))

__II__(15, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5); __II_C__(6); __II_C__(7);
       __II_C__(8);
       __II_C__(9);
       __II_C__(10);
       __II_C__(11);
       __II_C__(12);
       __II_C__(13);
       __II_C__(14))

__II__(16, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5); __II_C__(6); __II_C__(7);
       __II_C__(8);
       __II_C__(9);
       __II_C__(10);
       __II_C__(11);
       __II_C__(12);
       __II_C__(13);
       __II_C__(14);
       __II_C__(15))

__II__(17, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5); __II_C__(6); __II_C__(7);
       __II_C__(8);
       __II_C__(9);
       __II_C__(10);
       __II_C__(11);
       __II_C__(12);
       __II_C__(13);
       __II_C__(14);
       __II_C__(15);
       __II_C__(16))

__II__(18, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5); __II_C__(6); __II_C__(7);
       __II_C__(8);
       __II_C__(9);
       __II_C__(10);
       __II_C__(11);
       __II_C__(12);
       __II_C__(13);
       __II_C__(14);
       __II_C__(15);
       __II_C__(16);
       __II_C__(17))

__II__(19, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5); __II_C__(6); __II_C__(7);
       __II_C__(8);
       __II_C__(9);
       __II_C__(10);
       __II_C__(11);
       __II_C__(12);
       __II_C__(13);
       __II_C__(14);
       __II_C__(15);
       __II_C__(16);
       __II_C__(17);
       __II_C__(18))

__II__(20, __II_C__(0); __II_C__(1); __II_C__(2); __II_C__(3); __II_C__(4); __II_C__(5); __II_C__(6); __II_C__(7);
       __II_C__(8);
       __II_C__(9);
       __II_C__(10);
       __II_C__(11);
       __II_C__(12);
       __II_C__(13);
       __II_C__(14);
       __II_C__(15);
       __II_C__(16);
       __II_C__(17);
       __II_C__(18);
       __II_C__(19))

#undef __II__
#undef __II_C__

template <typename Fn, typename Arg>
struct front_binder {
  Fn&&  fn;
  Arg&& arg;
  template <typename... Args>
  constexpr decltype(auto) operator()(Args&&... args) const
  {
    return std::forward<Fn>(fn)(std::forward<Arg>(arg), std::forward<Args>(args)...);
  }
};

template <typename Fn, typename Arg>
inline constexpr front_binder<Fn, Arg> bind_front(Fn&& fn, Arg&& arg)
{
  return {std::forward<Fn>(fn), std::forward<Arg>(arg)};
}

template <typename... T, std::size_t... Is>
inline constexpr auto tuple_pop_front_impl(std::tuple<T...> const& tuple, std::index_sequence<Is...>)
{
  return std::make_tuple(std::get<Is + 1>(tuple)...);
}

template <typename... T>
inline constexpr auto tuple_pop_front(std::tuple<T...> const& tuple)
{
  return tuple_pop_front_impl(tuple, std::make_index_sequence<sizeof...(T) - 1>{});
}

template <std::size_t...>
struct composed_invoke_impl;

template <std::size_t N, std::size_t... Is>
struct composed_invoke_impl<N, Is...> {

  template <typename I, typename Function, typename... IndexTypes, typename... Args>
  constexpr decltype(auto) operator()(I, std::tuple<IndexTypes...> const& it, Function&& function, Args&&... args)
  {
    auto const fn = bind_front(std::forward<Function>(function), I{});
    return m_indexed_invoke<N>(
        std::get<0>(it), composed_invoke_impl<Is...>{}, tuple_pop_front(it), fn, std::forward<Args>(args)...);
  }

  template <typename I, typename Function, typename IndexType, typename... Args>
  constexpr decltype(auto) operator()(I, std::tuple<IndexType> const& it, Function&& function, Args&&... args)
  {
    auto const fn = bind_front(std::forward<Function>(function), I{});
    return m_indexed_invoke<N>(std::get<0>(it), fn, std::forward<Args>(args)...);
  }
};

template <>
struct composed_invoke_impl<> {
  template <typename I, typename Function, typename... Args>
  constexpr decltype(auto) operator()(I, std::tuple<> const& it, Function&& function, Args&&... args)
  {
    return std::forward<Function>(function)(I{}, std::forward<Args>(args)...);
  }
};
}  // namespace detail

template <std::size_t N, typename Function, typename... Args>
inline constexpr decltype(auto) m_indexed_invoke(std::size_t index, Function&& function, Args&&... args)
{
  SMP_ASSERT(index < N);
  return detail::indexed_invoke<N>::template invoke<0>(
      index, std::forward<Function>(function), std::forward<Args>(args)...);
}

template <std::size_t N, std::size_t... Ns, typename I1, typename... Is, typename Function, typename... Args>
inline constexpr decltype(auto)
m_multi_indexed_invoke(std::tuple<I1, Is...> const& indexes, Function&& function, Args&&... args)
{
  static_assert(sizeof...(Ns) == sizeof...(Is), "");
  return m_indexed_invoke<N>(
      std::get<0>(indexes),
      detail::composed_invoke_impl<Ns...>{},
      detail::tuple_pop_front(indexes),
      std::forward<Function>(function),
      std::forward<Args>(args)...);
}

}  // namespace mp

}  // namespace simple

#endif  // SIMPLE_MP_DETAIL_M_INDEXED_INVOKE_HPP
