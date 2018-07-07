#pragma once

#include <type_traits>

namespace Kernel
{
	const double Epsilon = 1e-6;

	template <typename T>
	bool fcomp(T lhs, T rhs, T epsilon = (T)Epsilon)
	{
		static_assert(std::is_floating_point<T>::value);
		return std::abs(rhs - lhs) < Epsilon;
	}
}