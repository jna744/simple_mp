#include <iostream>

#include <simple/mp.hpp>
#include <simple/mp/type_name.hpp>

using namespace simple::mp;

using L1 = std::tuple<void, int, float, void, int>;
using L2 = std::tuple<char[1], char[2], char[3], char[4], char[5]>;

template <class T1, class T2>
using first_is_void = std::is_same<T1, void>;
template <class T1, class T2>
using second = T2;

using R1 = m_transform_if<first_is_void, second, L1, L2>;

template<typename T>
using IsChar5 = std::is_same<T, char[5]>;

struct S {
};

using L3 = std::tuple<S, S>;

int main()
{
  using I = m_find<L3, int>;
  using I2 = m_same<I, m_size<L3>>;

  using C = m_contains<L3, int>;
  static_assert(!C::value, "");

  using L1 = m_list<int, double, int, int, float, double>;
  using L1U = m_make_unique<L1>;

  using IsUnique = m_is_unique<L1>;

  std::cout << m_type_name<IsUnique>() << std::endl;
  std::cout << m_type_name<L1U>() << std::endl;

  m_invoke_with_index<10>(5, [](auto I) {});

  using Result = m_any_of<L1, IsChar5>;

  // static_assert(Result{}, "");
}
