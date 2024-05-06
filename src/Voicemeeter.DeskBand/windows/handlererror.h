#pragma once

#include "windowserror.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class handler_error : public windows_error {
				const LRESULT m_result;

			public:
				handler_error(LRESULT result_arg, const char* what_arg) noexcept;

				inline LRESULT result() const noexcept {
					return m_result;
				};
			};
		}
	}
}