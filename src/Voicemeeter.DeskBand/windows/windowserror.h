#pragma once

#include <stdexcept>

#include <windows.h>

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class windows_error : public std::runtime_error {
				const DWORD m_errCode;

			public:
				windows_error(DWORD code_arg, const char* what_arg) noexcept;
				explicit windows_error(const char* what_arg) noexcept;

				inline DWORD code() const noexcept {
					return m_errCode;
				};
			};
		}
	}
}