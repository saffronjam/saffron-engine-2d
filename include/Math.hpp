#pragma once
#include <cmath>

constexpr double PI_D = 3.141592653589793238462643383279;
constexpr float PI = float(PI_D);

template <typename T>
auto sq(T x)
{
	return x * x;
}