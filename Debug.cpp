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

std::string Kernel::Exception::GetLocation() const
{
	std::ostringstream oss;
	
	if (_line != 0)
	{
		const char* trimmed = strrchr(_file, '\\');
		trimmed = trimmed ? trimmed + 1 : _file;
		oss << '[' << trimmed << ':' << _line << ']';
	}
	return oss.str();
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

void Kernel::Verify(const void* condition, const Kernel::Exception& e)
{
	Kernel::Verify(condition != nullptr, e);
}

void Kernel::Verify(bool condition, const Kernel::Exception& e)
{
	if (!KERNEL_ASSERT(condition))
		e.Raise();
}
