

#include <iostream>

#include <smp/smp.hpp>
using namespace smp;
// using namespace smp::placeholders;

template <typename T>
void print()
{
  std::cout << typeid(T).name() << std::endl;
}

template <typename T>
using add_one = m_size_t<T::value + 1>;

template <typename T>
using multiply_two = m_size_t<T::value * 2>;

template <typename T>
using subtract_one = m_size_t<T::value - 1>;

template <typename T>
using multiple_four = m_size_t<T::value * 4>;

using add_one_and_multiply_two = m_compose<add_one, multiply_two, subtract_one, multiple_four>;

int main()
{

  using L1 = m_list<int, double, float>;

  using L2 = m_replace_first<L1, std::string>;

  using Result = m_invoke_q<add_one_and_multiply_two, m_size_t<0>>;

  using DoubleIdx = m_find<L1, std::string>;

  static_assert(DoubleIdx::value == 3, "");

  constexpr auto const* name = m_type_name<int>();
  std::cout << std::string(name).size() << std::endl;

  std::cout << name << std::endl;

  std::cout << name << std::endl;

  using Map = m_map<m_list<int>, m_list<double>>;
  using Map2 = m_map_insert<Map, int>;

  using MapHasInt = m_map_contains<Map2, std::string>;

  std::cout << m_type_name<m_map_get<m_map_insert<Map2, std::string, int, int, int>, std::string>>() << std::endl;

  //  std::cout << "Name: " << name << "\nWorld size: " << world_size << "\nWorld index: " << world_index << std::endl;
}
