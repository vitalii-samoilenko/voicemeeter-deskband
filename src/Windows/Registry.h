#pragma once

#include <string>
#include <utility>

#include <windows.h>

namespace Windows {
	namespace Registry {
		template<typename T>
		bool TryGetValue(HKEY hKey, const ::std::wstring& subKey, const ::std::wstring& valueName, T& value) {
			T temp{};
			DWORD flag{};
			if (typeid(T) == typeid(DWORD)) {
				flag = RRF_RT_DWORD;
			} else {
				return false;
			}
			DWORD size{ sizeof(T) };
			for (;;) {
				switch (RegGetValueW(hKey, subKey.c_str(), valueName.c_str(), flag, NULL, &temp, &size)) {
				case ERROR_SUCCESS: {
					value = ::std::move(temp);
				} return true;
				case ERROR_FILE_NOT_FOUND: {
					if (flag & RRF_SUBKEY_WOW6432KEY) {
						return false;
					}
					flag |= RRF_SUBKEY_WOW6432KEY;
				} break;
				default: {

				} return false;
				}
			}
		}

		template<>
		bool TryGetValue(HKEY hKey, const ::std::wstring& subKey, const ::std::wstring& valueName, ::std::wstring& value);
	}
}