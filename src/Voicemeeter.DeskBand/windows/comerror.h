#pragma once

#include <stdexcept>

#include <windows.h>

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class com_error : public std::runtime_error {
				const HRESULT m_errCode;

			public:
				com_error(HRESULT code_arg, const char* what_arg) noexcept;

				inline HRESULT code() const noexcept {
					return m_errCode;
				};
			};

			inline bool ThrowIfFailed(HRESULT code, const char* what, HRESULT ignore) {
				if (FAILED(code)) {
					if (code == ignore) {
						return true;
					}
					throw com_error{ code, what };
				}
				return false;
			}

			inline void ThrowIfFailed(HRESULT code, const char* what) {
				ThrowIfFailed(code, what, S_OK);
			}
		}
	}
}