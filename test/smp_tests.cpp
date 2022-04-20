#include <iostream>

#include <simple/mp/type_name.hpp>

#include <simple/mp.hpp>

using namespace simple::mp;
using std::tuple;

using L1 = std::tuple<int, std::tuple<>, void, std::tuple<float, double>>;
using R1 = m_flatten<L1>;  // tuple<int, void, float, double>

using L2 = m_list<int, m_list<float>, tuple<void>>;
using R2a = m_flatten<L2>;           // mp_list<int, float, tuple<void>>
using R2b = m_flatten<L2, tuple<>>;  // mp_list<int, mp_list<float>, void>

using L3 = m_list<m_list<float>, m_list<m_list<void>>>;
using R3 = m_flatten<L3>;  // mp_list<float, mp_list<void>>
                           //
int main()
{
  std::cout << "R1:" << m_type_name<R1>() << std::endl;
  std::cout << "R2a: " << m_type_name<R2a>() << std::endl;
  std::cout << "R2b: " << m_type_name<R2b>() << std::endl;
  std::cout << "R3 " << m_type_name<R3>() << std::endl;
  // m_indexed_vtable_invoke_c<10>(5, [](auto I) {});
  m_indexed_invoke<10>(5, [](auto I) { std::cout << I.value << std::endl; });

  constexpr auto ret = m_multi_indexed_invoke<10, 5, 2>(
      std::make_tuple(1, 0, 1), [](auto I1, auto I2, auto I3) { return I1.value + I2.value + I3.value; });
  std::cout << ret << std::endl;
}
