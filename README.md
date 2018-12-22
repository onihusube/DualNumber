# DualNumber

constexprな双対数（二重数）の実装

~~~C++
//Header only
#include"DualNumber.hpp"

constexpr DualNumbers::dual<double> d1{}; //{0.0, 0.0}
constexpr DualNumbers::dual<double> d2{ 3.0 };  //{3.0, 0.0}
constexpr DualNumbers::dual<double> d3 = { 1.0, 1.0 };  //{1.0, 0.0}

//Arithmetic
constexpr auto add = d1 + d2;  //{3.0, 0.0}
constexpr auto sub = d2 - d3;  //{2.0, -1.0}
constexpr auto mul = d2 + d3;  //{4.0, 1.0}
constexpr auto div = d3 - d2;  //{-2.0, 1.0}

//f(x) = 4x^3 + 3x^2 + 2x + 1
//df(x) = 12x^2 + 6x + 2
constexpr auto d5 = 4.0*d3*d3*d3 + 3.0*d3*d3 + 2.0*d3 + 1.0; //{10.0, 20.0}

constexpr auto inv = d5.inverted(); //{0.1..., -1999.9999...}
constexpr auto conj = d5.conjugated(); //{10.0, -20.0}

//Using directive
using namespace DualNumbers;

//User-defined literals
constexpr auto literal1 = 1.0_d;
constexpr auto literal2 = 3.5_eps;
constexpr auto literal3 = 1.25_d + 2.34_eps;

//Equality comparison
constexpr bool eq = d1 == d1; //true
constexpr bool neq = d1 != d2;//true

//Lexicographical order
constexpr bool lt = d1 < d2;  //true
constexpr bool ltq = d1 <= d1;//true
constexpr bool gt = d3 > d2;  //false
constexpr bool gtq = d3 >= d2;//false
~~~
