#ifndef SIMPLE_MP_MAP_HPP
#define SIMPLE_MP_MAP_HPP

#include <simple/mp/detail/config.hpp>

#include <simple/mp/bind.hpp>
#include <simple/mp/list.hpp>
#include <simple/mp/utility.hpp>

namespace simple
{

namespace mp
{

namespace detail
{

template <typename K, typename L>
struct m_map_key_value {
  using key = K;
  using value = L;
};

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

template <typename KVP, typename Key>
using m_map_same_key = m_same<m_first<KVP>, Key>;

template <typename Map, typename KVP>
using m_map_update = m_transform_if_q<m_bind_back<m_map_same_key, m_first<KVP>>, m_bind_back<m_assign, KVP>, Map>;

}  // namespace detail

template <typename>
struct m_is_map : m_false {
};

template <typename... Ts>
struct m_is_map<detail::m_map_impl<Ts...>> : m_true {
};

template <typename... Ts>
using m_map = m_t_<detail::m_map_impl<Ts...>>;

template <typename Map, typename Key>
using m_map_contains = m_base_of<m_identity<Key>, m_assign<m_inherit<>, typename Map::keys>>;

template <typename Map, typename Key, typename... Values>
using m_map_insert = m_eval_unless<m_map_contains<Map, Key>, Map, m_push_back, Map, m_list<Key, Values...>>;

template <typename Map, typename Key, typename... Values>
using m_map_replace = m_t_<m_if<
    m_map_contains<Map, Key>,
    m_defer<detail::m_map_update, Map, m_list<Key, Values...>>,
    m_defer<m_push_back, Map, m_list<Key, Values...>>>>;

template <typename Map, typename Key>
using m_map_get = m_t_<detail::m_map_get_impl<Map, Key>>;

}  // namespace mp

}  // namespace simple

#endif  // SIMPLE_MP_MAP_HPP
