#include "MessageBox.h"
#include "Util.h"

#include <windows.h>
#undef MessageBox

using Kernel::MessageBox;

MessageBox::MessageBox(void* parent, const std::string& title) : m_parent(parent), m_title(StringToWString(title)) {}

MessageBox::Button MessageBox::Show(const std::string& message, Type type)
{
	DWORD types[] = { MB_OK, MB_OKCANCEL, MB_YESNOCANCEL };

	switch (::MessageBoxW(reinterpret_cast<HWND>(m_parent), StringToWString(message).c_str(), m_title.c_str(), types[(int)type]))
	{
	case IDOK: return Button::OK;
	case IDYES: return Button::Yes;
	case IDNO: return Button::No;
	}

	return Button::Cancel;
}
