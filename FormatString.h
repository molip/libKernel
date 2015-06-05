#pragma once

#include "Debug.h"

#include <vector>
#include <string>
#include <cctype>
#include <sstream>

namespace Kernel
{
	void _FormatStringPush(std::vector<std::string>& vec);

	template<typename T, typename... Targs>
	void _FormatStringPush(std::vector<std::string>& vec, T value, Targs... args)
	{
		std::ostringstream oss;
		oss << value;
		vec.push_back(oss.str());
		_FormatStringPush(vec, args...);
	}

	template<typename T0, typename T1, typename T2, typename T3, typename T4,
		typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename... Targs>
		void _FormatStringPush(std::vector<std::string>& vec, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, Targs... args)
	{
			static_assert(false, "FormatString: Too many parameters (max 10)");
	}

	template<typename... Targs>
	std::string FormatString(const char* format, Targs... args)
	{
		std::vector<std::string> vec;
		_FormatStringPush(vec, args...);

		std::ostringstream oss;
		bool bToken = false;
		for (; *format != '\0'; ++format)
		{
			if (bToken)
			{
				bToken = false;
				if (::isdigit(*format))
				{
					char num[] = { *format, '\0' };
					int i = ::atoi(num);
					KERNEL_ASSERT(i < (int)vec.size());
					oss << vec[i];
				}
				else
				{
					KERNEL_ASSERT(*format == '%');
					oss << *format;
				}
			}
			else if (*format == '%')
				bToken = true;
			else
				oss << *format;
		}

		return oss.str();
	}

	template<typename... Targs>
	std::string FormatString(const std::string& format, Targs... args)
	{
		return FormatString(format.c_str(), args...);
	}
}