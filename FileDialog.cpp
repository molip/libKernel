#include "FileDialog.h"
#include "Util.h"

#include <windows.h>
#include <Shlobj.h>

void Kernel::FileDialog::SetDefaultExtension(const std::string & ext) 
{
	m_defaultExt = StringToWString(ext); 
}

void Kernel::FileDialog::AddFileTypeFilter(const std::string & name, const std::string & pattern) 
{
	m_fileTypes.push_back(std::make_pair(StringToWString(name), StringToWString(pattern)));
}

void Kernel::FileDialog::AddAllTypesFilter()
{
	AddFileTypeFilter("All files", "*.*");
}

bool Kernel::FileDialog::Show()
{
	IFileDialog* pfd = nullptr;
	HRESULT hr = CoCreateInstance(m_type == Type::Save ? CLSID_FileSaveDialog : CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
	if (!SUCCEEDED(hr))
		return false;

	if (!m_fileTypes.empty())
	{
		std::vector<COMDLG_FILTERSPEC> filters;
		for (auto&[name, pattern] : m_fileTypes)
			filters.push_back({ name.c_str(), pattern.c_str() });

		hr = pfd->SetFileTypes((int)filters.size(), &filters.front());
		hr = pfd->SetFileTypeIndex(1);
	}

	if (!m_defaultExt.empty())
		hr = pfd->SetDefaultExtension(m_defaultExt.c_str());

	hr = pfd->Show(reinterpret_cast<HWND>(m_parent));
	if (SUCCEEDED(hr))
	{
		std::wstring path;
		
		IShellItem *psiResult;
		hr = pfd->GetResult(&psiResult);
		if (SUCCEEDED(hr))
		{
			PWSTR pszFilePath = NULL;
			hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
			if (SUCCEEDED(hr))
			{
				m_path = pszFilePath;
				CoTaskMemFree(pszFilePath);
			}
			psiResult->Release();
		}
	}
	pfd->Release();

	return !m_path.empty();
}
