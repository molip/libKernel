#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "Util.h"

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

std::string Kernel::GetTimeStamp()
{
	time_t now = ::time(nullptr);
	tm local;
	localtime_s(&local, &now);

	std::ostringstream oss;
	oss << std::put_time(&local, "%Y%m%d_%H%M%S");
	return oss.str();
}
