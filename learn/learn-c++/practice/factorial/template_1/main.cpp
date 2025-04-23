// NOTE: Only compatible with C++17 or later versions.
#include <iostream>
#include <type_traits>
#include <utility>

namespace {

    template <typename T> struct FactorialHelper;

    template <long unsigned int... Ns>
    struct FactorialHelper<std::integer_sequence<long unsigned int, Ns...>> :
    public std::integral_constant<long unsigned int, ((Ns + 1) * ...)> {};
template <long unsigned int N>
using Factorial = FactorialHelper<
        std::make_integer_sequence<long unsigned int, N>>;

}  // namespace

int main(int argc, char* argv[]) {
    std::cout << Factorial<20>::value << std::endl;
    return 0;
}