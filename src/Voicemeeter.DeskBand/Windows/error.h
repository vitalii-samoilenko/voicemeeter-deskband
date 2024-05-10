#pragma once

#include <stdexcept>

#include <windows.h>

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class windows_error : public std::runtime_error {
			public:
				inline windows_error(DWORD code_arg, const char* what_arg) noexcept
					: runtime_error{ what_arg }
					, m_errCode{ code_arg } {

				};
				inline explicit windows_error(const char* what_arg) noexcept
					: windows_error{ GetLastError(), what_arg } {

				};

				inline DWORD code() const noexcept {
					return m_errCode;
				};

			private:
				const DWORD m_errCode;
			};

			class com_error : public std::runtime_error {
			public:
				inline com_error(HRESULT code_arg, const char* what_arg) noexcept
					: runtime_error{ what_arg }
					, m_errCode{ code_arg } {

				};

				inline HRESULT code() const noexcept {
					return m_errCode;
				};

			private:
				const HRESULT m_errCode;
			};
		}
	}
}