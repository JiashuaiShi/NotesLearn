#include <iostream>
#include <utility>

template<uint64_t N>
constexpr uint64_t fact = fact<N - 1> * N;

template<>
constexpr uint64_t fact<0> = 1;

template<std::size_t...I>
constexpr auto foo(std::index_sequence<I...>)
{
    return ((I+1) * ...);
}

int main()
{
    std::cout << foo(std::make_index_sequence<20>()) << std::endl;
}
