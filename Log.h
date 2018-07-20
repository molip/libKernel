#pragma once

#include <sstream>
#include <fstream>

namespace Kernel
{
	class _Logger : public std::ofstream
	{
	public:
		bool Init(const std::wstring& path);

		//_Logger& operator<< (basic_ostream& (*pf)(basic_ostream&)) { __super::operator<<(pf); return *this; } // For std::endl.
		//_Logger& operator<< (const char* val) { __super::operator<<(val); return *this; }
	};

	_Logger& Log(int indent = 0);
}

std::ostream& operator<< (std::ostream& stream, const std::wstring& val);
