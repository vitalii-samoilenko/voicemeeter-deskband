#ifndef WINDOWS_COM_HPP
#define WINDOWS_COM_HPP

#include <type_traits>

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

	template<typename T,
		::std::enable_if_t<
			::std::is_pointer_v<T>,
			bool> = true>
	inline void SafeRelease(T &obj) {
		if (obj) {
			obj->Release();
			obj = nullptr;
		}
	};
	template<typename T,
		::std::enable_if_t<
			!::std::is_pointer_v<T>,
			bool> = true>
	inline void SafeRelease(T &objs) {
		for (auto &obj : objs) {
			SafeRelease(obj);
		}
	};
}

#endif
