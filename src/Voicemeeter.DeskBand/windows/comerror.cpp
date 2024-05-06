#include "comerror.h"

#include <windows.h>

using namespace Voicemeeter::DeskBand::Windows;

com_error::com_error(HRESULT code_arg, const char* what_arg) noexcept
	: runtime_error{ what_arg }
	, m_errCode{ code_arg } {

	}