#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "Util.h"

#include <algorithm>
#include <iomanip>
#include <codecvt>
#include <locale>
#include <sstream>
#include <time.h>

std::wstring Kernel::StringToWString(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
}

std::string Kernel::WStringToString(const std::wstring & str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(str);
}

namespace
{
	template <typename T, typename F> 
	T Transform(const T& src, F f)
	{
		T dst;
		dst.resize(src.size());
		std::transform(src.begin(), src.end(), dst.begin(), f);
		return dst;
	}
}

std::string Kernel::ToLower(const std::string& str)
{
	return Transform(str, ::tolower);
}

std::wstring Kernel::ToLower(const std::wstring& str)
{
	return Transform(str, ::towlower);
}

std::string Kernel::ToUpper(const std::string& str)
{
	return Transform(str, ::toupper);
}

std::wstring Kernel::ToUpper(const std::wstring& str)
{
	return Transform(str, ::towupper);
}

std::string Kernel::GetTimeStamp()
{
	time_t now = ::time(nullptr);
	tm local;
	localtime_s(&local, &now);

	std::ostringstream oss;
	oss << std::put_time(&local, "%Y%m%d_%H%M%S");
	return oss.str();
}
