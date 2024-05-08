#pragma once

#include <stdexcept>

#include <windows.h>

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class windows_error : public std::runtime_error {
				const DWORD m_errCode;

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
			};

			class handler_error : public windows_error {
				const LRESULT m_result;

			public:
				inline handler_error(LRESULT result_arg, const char* what_arg) noexcept
					: windows_error{ what_arg }
					, m_result{ result_arg } {

					};

				inline LRESULT result() const noexcept {
					return m_result;
				};
			};

			class com_error : public std::runtime_error {
				const HRESULT m_errCode;

			public:
				inline com_error(HRESULT code_arg, const char* what_arg) noexcept
					: runtime_error{ what_arg }
					, m_errCode{ code_arg } {

					};

				inline HRESULT code() const noexcept {
					return m_errCode;
				};
			};
		}
	}
}