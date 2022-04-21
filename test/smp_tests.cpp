#include <iostream>

#include <simple/mp.hpp>

using namespace simple::mp;

int main()
{
  auto ret = m_indexed_invoke<20, 52, 100>(
      std::make_tuple(16, 50, 99),
      [](auto I1, auto I2, auto I3, auto const* msg)
      {
        std::cout << msg << std::endl;
        return I1.value + I2.value + I3.value;
      },
      "Hello");
  std::cout << ret << std::endl;
  // auto ret = m_indexed_invoke<2546>(732, [](auto I) { return I.value; });
  // std::cout << ret << std::endl;
}
