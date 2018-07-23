#include "Debug.h"

#include <Windows.h>

#define ENABLE_TRACE

namespace Kernel 
{
	namespace Debug
	{
		_Tracer Trace;
	}
}

using namespace Kernel;
using namespace Kernel::Debug;

bool _Tracer::Enabled() const
{
#ifdef ENABLE_TRACE
	return true;
#else 
	return false;
#endif
}

void _Tracer::Output(const std::string& str)
{
	::OutputDebugString(str.c_str());
}

using namespace std::literals;

bool Kernel::DebugBreak() { ::DebugBreak(); return false; }

namespace
{
	std::string GetLocationString(const char* file, int line)
	{
		std::ostringstream oss;

		if (line != 0)
		{
			const char* trimmed = strrchr(file, '\\');
			trimmed = trimmed ? trimmed + 1 : file;
			oss << '[' << trimmed << ':' << line << ']';
		}
		return oss.str();
	}
}

std::string Kernel::Exception::GetLocation() const
{
	return GetLocationString(_file, _line);
}

const char* Kernel::Exception::what() const
{
	if (_what.empty())
		_what = GetWhat();

	return _what.c_str();
}

std::string Kernel::Exception::GetWhat() const
{
	std::string s = "Exception at "s + GetLocation();
	if (!_msg.empty())
		s += ": "s + _msg;
	return s;
}

void Kernel::Verify(const void* condition, const char* file, int line)
{
	Kernel::Verify(condition != nullptr, file, line);
}

void Kernel::Verify(bool condition, const char* file, int line)
{
	if (!condition)
	{
		if (::IsDebuggerPresent())
			DebugBreak();
		
		::FatalAppExitA(0, (std::string("Assertion failed: ") + GetLocationString(file, line)).c_str());
	}
}
