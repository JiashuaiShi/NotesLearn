#include <iostream>
#include <iomanip>
#include <type_traits>

using BaseType_t = long long;
constexpr BaseType_t lgBase = 9; // 注意10000*10000刚刚好小于int的取值范围
constexpr BaseType_t Base = 1000000000; // 注意10000*10000刚刚好小于int的取值范围

// 大整数的表示
template<BaseType_t...I> struct BigInteger {
    using type = BigInteger;
};

// 连接
template<class T1, class T2> struct BI_Cat;
template<BaseType_t...I1, BaseType_t...I2> struct BI_Cat <BigInteger<I1...>, BigInteger<I2...>> : BigInteger<I1..., I2...> {};

// 左移一个单元（即*Base）
template<class T> struct BI_SHL;
template<BaseType_t...I> struct BI_SHL<BigInteger<I...>> : BigInteger<I..., 0> {};

// 去除开头的0
template<class T> struct BI_Remove_Zeros : T {};
template<BaseType_t...I> struct BI_Remove_Zeros<BigInteger<0, I...>> : BI_Remove_Zeros<BigInteger<I...>> {};

// 填充0到N个单元
template<int X, class IS> struct BI_Fill_Impl;
template<int X, class T, T...I> struct BI_Fill_Impl<X, std::integer_sequence<T, I...>> : BigInteger<(I, X)...> {};
template<int Size> struct BI_Fill_Zeros : BI_Fill_Impl<0, std::make_index_sequence<Size>> {};

template<class T, int N> struct BI_Resize;
template<BaseType_t...I, int N> struct BI_Resize<BigInteger<I...>, N> : BI_Cat<typename BI_Fill_Zeros<N - sizeof...(I)>::type, BigInteger<I...>> {};

// 返回较大的数值
template<int A, int B> struct int_min : std::integral_constant<int, (A<B?B:A)> {};

// 非进位加法：先把两个数的位数改成一样的然后依次相加
template<class A, class B, class ShouldResize> struct BI_AddNotCarry_Impl;
template<BaseType_t...I1, BaseType_t...I2> struct BI_AddNotCarry_Impl <BigInteger<I1...>, BigInteger<I2...>, std::true_type> : BigInteger<(I1 + I2)...> {};

template<BaseType_t...I1, BaseType_t...I2> struct BI_AddNotCarry_Impl <BigInteger<I1...>, BigInteger<I2...>, std::false_type>
        : BI_AddNotCarry_Impl<
                typename BI_Resize<BigInteger<I1...>, int_min<sizeof...(I1), sizeof...(I2)>::value>::type,
                typename BI_Resize<BigInteger<I2...>, int_min<sizeof...(I1), sizeof...(I2)>::value>::type,
                std::true_type
        >{};

template<class A, class B> struct BI_AddNotCarry;
template<BaseType_t...I1, BaseType_t...I2> struct BI_AddNotCarry <BigInteger<I1...>, BigInteger<I2...>>
        : BI_AddNotCarry_Impl<BigInteger<I1...>, BigInteger<I2...>, std::bool_constant<sizeof...(I1) == sizeof...(I2)>> {};

// 判断是否为0
template<class Y> struct BI_IsZero;
template<BaseType_t...I> struct BI_IsZero<BigInteger<I...>> : std::bool_constant<((I == 0) && ...)> {};

// 自动进位
template<class A> struct BI_Carry;
template<class A, class B> struct BI_Add : BI_Carry<typename BI_AddNotCarry<A, B>::type> {};

template<class Mod, class Div, class ShouldCalc = typename BI_IsZero<Div>::type> struct BI_Carry_Impl;
template<class Mod, class Div> struct BI_Carry_Impl<Mod, Div, std::true_type> : Mod {};
template<class Mod, class Div> struct BI_Carry_Impl<Mod, Div, std::false_type>
        : BI_Add<Mod, typename BI_SHL<Div>::type > {};
template<BaseType_t...I> struct BI_Carry<BigInteger<I...>>
        : BI_Remove_Zeros<typename BI_Carry_Impl<BigInteger<(I % Base)...>, BigInteger<(I / Base)...>>::type> {};

// 乘以X并自动进位
template<class A, int X> struct BI_MulX;
template<BaseType_t...I1, int X> struct BI_MulX <BigInteger<I1...>, X>
        : BI_Carry<BigInteger<(I1 * X)...>> {};

// 计算阶乘
template<int X> struct BI_Fact : BI_MulX<typename BI_Fact<X-1>::type, X> {};
template<> struct BI_Fact<0> : BigInteger<1> {};

template<BaseType_t...I>
std::ostream &operator<<(std::ostream &out, BigInteger<I...>) {
    return ((out << std::setfill('0') << I << std::setw(lgBase)), ...);
}

int main()
{
    std::cout << typename BI_Fact<20>::type() << std::endl;
}