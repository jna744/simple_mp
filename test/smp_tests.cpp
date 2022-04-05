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
  std::cout << m_type_name<R1>() << std::endl;
}
