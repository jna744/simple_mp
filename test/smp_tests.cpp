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

int main()
{

  using L1 = m_list<int, double, int, int, float, double>;
  using L1U = m_make_unique<L1>;

  using IsUnique = m_is_unique<L1>;

  std::cout << m_type_name<IsUnique>() << std::endl;
  std::cout << m_type_name<L1U>() << std::endl;

  m_invoke_with_index<10>(5, [](auto I) {});

  static_assert(m_count<L1, int>::value == 3, "");
}
