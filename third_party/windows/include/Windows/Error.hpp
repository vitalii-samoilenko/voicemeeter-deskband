#ifndef WINDOWS_ERROR_HPP
#define WINDOWS_ERROR_HPP

#include <stdexcept>

#include <windows.h>

namespace Windows {
	class Error : public ::std::runtime_error {
	public:
		inline Error(DWORD code_arg, char const *what_arg) noexcept
			: runtime_error{ what_arg }
			, _code{ code_arg } {

		};
		inline explicit Error(char const *what_arg) noexcept
			: Error{ ::GetLastError(), what_arg } {

		};

		inline DWORD code() const noexcept {
			return _code;
		};

	private:
		DWORD _code;
	};
}

#endif
