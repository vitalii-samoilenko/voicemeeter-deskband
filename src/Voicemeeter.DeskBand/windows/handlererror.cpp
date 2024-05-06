#include "handlererror.h"

using namespace Voicemeeter::DeskBand::Windows;

handler_error::handler_error(LRESULT result_arg, const char* what_arg) noexcept
	: windows_error{ what_arg }
	, m_result{ result_arg } {

	}