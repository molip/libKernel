#pragma once

#include <string>
#include <vector>

namespace Kernel::FileSystem
{
	std::vector<std::wstring> FindFilesInDir(const std::wstring& dir, const std::wstring& pattern = L"*");
	bool DeleteFile(const std::wstring& path);
	bool CreateDir(const std::wstring& path, bool failIfExists = false);
	std::wstring GetUserDocumentsDir();
}