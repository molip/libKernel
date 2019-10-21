#pragma once

#include "Debug.h"

#include <vector>
#include <string>
#include <cctype>
#include <sstream>

namespace Kernel
{
	void _FormatStringPush(std::vector<std::string>& vec);
	void _FormatStringPush(std::vector<std::wstring>& vec);

	template<typename Tchar>
	using StringT = std::basic_string<Tchar, std::char_traits<Tchar>, std::allocator<Tchar>>;

	template<typename Tchar>
	using StreamT = std::basic_ostringstream<Tchar, std::char_traits<Tchar>, std::allocator<Tchar>>;

	template<typename Tchar, typename T, typename... Targs>
	void _FormatStringPush(std::vector<StringT<Tchar>>& vec, T value, Targs... args)
	{
		std::basic_ostringstream<Tchar, std::char_traits<Tchar>, std::allocator<Tchar>> oss;
		oss << value;
		vec.push_back(oss.str());
		_FormatStringPush(vec, args...);
	}

	template<typename Tchar, typename T0, typename T1, typename T2, typename T3, typename T4,
		typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename... Targs>
		void _FormatStringPush(std::vector<StringT<Tchar>>& vec, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, Targs... args)
	{
		static_assert(false, "FormatString: Too many parameters (max 10)");
	}

	template<typename Tchar, typename... Targs>
	StringT<Tchar> FormatString(const Tchar* format, Targs... args)
	{
		std::vector<StringT<Tchar>> vec;
		_FormatStringPush(vec, args...);

		StreamT<Tchar> oss;
		bool bToken = false;
		for (; *format != '\0'; ++format)
		{
			if (bToken)
			{
				bToken = false;
				if (::isdigit(*format))
				{
					Tchar num[] = { *format, 0 };
					size_t i = std::stoul(num);
					KERNEL_ASSERT(i < vec.size());
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

	template<typename... Targs>
	std::wstring FormatString(const std::wstring& format, Targs... args)
	{
		return FormatString(format.c_str(), args...);
	}
}