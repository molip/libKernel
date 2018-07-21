#pragma once

#include <string>

struct _GUID;

namespace Kernel
{
	class MessageBox
	{
	public:
		MessageBox(void* parent, const std::string& title = {});

		enum class Button { OK, Cancel, Yes, No };
		enum class Type { OK, OKCancel, YesNoCancel };

		Button Show(const std::string& message, Type type);
	private:
		void* m_parent;
		std::wstring m_title;
	};
}