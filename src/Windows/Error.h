#pragma once

#include <stdexcept>

#include <windows.h>

namespace Windows {
	class Error : public ::std::runtime_error {
	public:
		inline Error(DWORD code_arg, const char* what_arg) noexcept
			: runtime_error{ what_arg }
			, m_errCode{ code_arg } {

		};
		inline Error(HRESULT code_arg, const char* what_arg) noexcept
			: runtime_error{ what_arg }
			, m_errCode{ code_arg } {

		};
		inline explicit Error(const char* what_arg) noexcept
			: Error{ GetLastError(), what_arg } {

		};

		inline long long code() const noexcept {
			return m_errCode;
		};

	private:
		const long long m_errCode;
	};
}