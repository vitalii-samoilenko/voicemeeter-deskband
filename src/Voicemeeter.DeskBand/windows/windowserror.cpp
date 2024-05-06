#include "windowserror.h"

#include <windows.h>

using namespace Voicemeeter::DeskBand::Windows;

windows_error::windows_error(DWORD code_arg, const char* what_arg) noexcept
	: runtime_error{ what_arg }
	, m_errCode{ code_arg } {

	}

windows_error::windows_error(const char* what_arg) noexcept
	: windows_error{ GetLastError(), what_arg } {

	}