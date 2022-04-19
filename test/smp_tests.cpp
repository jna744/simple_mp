#include <iostream>

#include <simple/mp/type_name.hpp>

#include <simple/mp.hpp>

using namespace simple::mp;

int main()
{
  auto ret = m_invoke_with_index_c<10>(5, [](auto i) { return i.value + 2; });
  std::cout << ret << std::endl;
}
