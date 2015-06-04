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
