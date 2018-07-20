#include "Filesystem.h"

#include <windows.h>
#include <Shlobj.h>

std::vector<std::wstring> Kernel::FileSystem::FindFilesInDir(const std::wstring& dir, const std::wstring& pattern)
{
	std::vector<std::wstring> files;

	std::wstring dir2 = dir;
	if (!dir2.empty() && dir2.back() != L'/')
		dir2.push_back(L'/');
	dir2 += pattern;

	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFileW(dir2.c_str(), &ffd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			files.push_back(ffd.cFileName);
		}
		while (FindNextFileW(hFind, &ffd) != 0);
 
		FindClose(hFind);
	}
	return files;
}
#undef DeleteFile
bool Kernel::FileSystem::DeleteFile(const std::wstring& path)
{
	return ::DeleteFileW(path.c_str()) != 0;
}

bool Kernel::FileSystem::CreateDir(const std::wstring& path, bool failIfExists)
{
	if (::CreateDirectoryW(path.c_str(), nullptr))
		return true;

	return !failIfExists && ::GetLastError() == ERROR_ALREADY_EXISTS;
}

std::wstring Kernel::FileSystem::GetUserDocumentsDir()
{
	wchar_t path[MAX_PATH];
	return SUCCEEDED(::SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path)) ? path : std::wstring();
}

