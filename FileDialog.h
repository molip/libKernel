#pragma once

#include <string>
#include <vector>

struct _GUID;

namespace Kernel
{
	class FileDialog
	{
	public:
		enum class Type { Save, Open };

		FileDialog(Type type, void* parent) : m_type(type), m_parent(parent) {}

		void SetDefaultExtension(const std::string& ext); // No dot.
		void AddFileTypeFilter(const std::string& name, const std::string& pattern);
		void AddAllTypesFilter();
		
		bool Show();

		const std::wstring& GetPath() const { return m_path; }
	
	private:
		Type m_type;
		void* m_parent;
		std::wstring m_path, m_defaultExt;
		std::vector<std::pair<std::wstring, std::wstring>> m_fileTypes;
	};
}