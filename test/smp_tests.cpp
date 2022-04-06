#include <iostream>

#include <smp/smp.hpp>

using namespace smp;

using L1 = std::tuple<void, int, float, void, int>;
using L2 = std::tuple<char[1], char[2], char[3], char[4], char[5]>;

template <class T1, class T2>
using first_is_void = std::is_same<T1, void>;
template <class T1, class T2>
using second = T2;

using R1 = m_transform_if<first_is_void, second, L1, L2>;

int main()
{

  using Map = m_map<>;

  using Map1 = m_map_insert<Map, int, double, float, std::string>;
  using Map2 = m_map_insert<Map1, double, int>;
  using Map3 = m_map_insert<Map2, std::string, int>;
  using Map4 = m_map_replace<Map3, int>;

  std::cout << m_type_name<Map4>() << std::endl;
  int x = 5;
  m_table_invoke<10>(5, [&x](auto I) { x = 10; });
  std::cout << x << std::endl;
}
