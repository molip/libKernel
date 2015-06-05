#pragma once

#include "Debug.h"

#include <string>

namespace Kernel
{
	template<typename T>
	class EnumTraits
	{
	public:
		static T FromString(const std::string& s)
		{
			return FromString(s.c_str());
		}

		static T FromString(const char* s)
		{
			for (int i = 0; i < *names[i] != 0; ++i)
				if (strcmp(names[i], s) == 0)
				return T(i + base);
			KERNEL_VERIFY(false);
			return T(0);
		}

		static const char* ToString(T t)
		{
			int i = int(t) - base;
			int n = 0;
			for (; *names[n] != 0; ++n);
			KERNEL_VERIFY(i >= 0 && i < n);
			return names[i];
		}
	private:
		static const char* names[];
		static int base;
	};

	template <typename T>
	const char* EnumToString(T t) { return EnumTraits<T>::ToString(t); }

	template <typename T>
	T EnumFromString(const char* s) { return EnumTraits<T>::FromString(s); }

	template <typename T>
	T EnumFromString(const std::string& s) { return EnumTraits<T>::FromString(s); }

	#define DEFINE_ENUM_NAMES(T) \
		int Kernel::EnumTraits<T>::base; const char* Kernel::EnumTraits<T>::names[] = 

	#define DEFINE_ENUM_NAMES2(T, b) \
		int Kernel::EnumTraits<T>::base = b; const char* Kernel::EnumTraits<T>::names[] = 
}