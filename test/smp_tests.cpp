#include <iostream>

#include <simple/mp/type_name.hpp>

#include <simple/mp.hpp>

using namespace simple::mp;

using L1 = std::tuple<m_list<int, void, double, m_list<int, int>>, m_list<float>, long long>;

int main()
{
  auto ret = m_invoke_with_index_c<10>(5, [](auto i) { return i.value + 2; });
  std::cout << ret << std::endl;
  std::cout << m_type_name<m_deep_flatten<L1>>() << std::endl;
}
