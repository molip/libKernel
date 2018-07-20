#include "Log.h"
#include "Util.h"

bool Kernel::_Logger::Init(const std::wstring& path)
{
	__super::open(path, std::ios::binary);
	return is_open();
}

std::ostream& operator<< (std::ostream& stream, const std::wstring& str)
{
	stream << Kernel::WStringToString(str).c_str();
	return stream;
}

Kernel::_Logger& Kernel::Log(int indent)
{
	static Kernel::_Logger instance;
	instance << std::string(indent, '\t');
	return instance;
}
