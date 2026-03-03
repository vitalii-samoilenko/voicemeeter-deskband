#ifndef WINDOWS_COM_HPP
#define WINDOWS_COM_HPP

#include "Windows/Error.hpp"

namespace Windows {
	inline HRESULT ThrowIfFailed(
		HRESULT code, 
		char const *what
	) {
		if (FAILED(code)) {
			throw Error{ static_cast<DWORD>(code), what };
		}
		return code;
	};
}

#endif
