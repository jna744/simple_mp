#ifndef SIMPLE_MP_TYPE_NAME_HPP
#define SIMPLE_MP_TYPE_NAME_HPP

#include <simple/mp/detail/config.hpp>

#include <simple/mp/numeric_constants.hpp>
#include <simple/mp/utility.hpp>

#include <utility>

namespace simple
{

namespace mp
{

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

SMP_INLINE(17) constexpr auto m_string_npos = std::size_t(-1);

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
  static SMP_INLINE(17) constexpr type value = m_construct_type_name_impl<T>();
};

#if SMP_CPP(14)
template <typename T>
constexpr typename m_construct_type_name<T>::type m_construct_type_name<T>::value;
#endif

}  // namespace detail

template <typename T>
inline constexpr char const* m_type_name() noexcept
{
  constexpr auto& name = detail::m_construct_type_name<T>::value;
  return name.value;
}

}  // namespace mp

}  // namespace simple

#endif  // SIMPLE_MP_TYPE_NAME_HPP
