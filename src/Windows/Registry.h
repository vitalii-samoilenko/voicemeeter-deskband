#pragma once

#include "cstring"
#include <string>
#include <utility>
#include <vector>

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
		template<typename T>
		bool TryGetValue(HKEY hKey, const ::std::wstring& subKey, const ::std::wstring& valueName, ::std::vector<T>& value) {
			::std::vector<char> buffer{ '\0' };
			DWORD flag{ RRF_RT_REG_BINARY };
			DWORD size{ sizeof(char) };
			for (;;) {
				switch (RegGetValueW(hKey, subKey.c_str(), valueName.c_str(), flag, NULL, buffer.data(), &size)) {
				case ERROR_SUCCESS: {
					if (size % sizeof(T)) {
						return false;
					}
					value.resize(size / sizeof(T));
					::std::memcpy(value.data(), buffer.data(), size);
				} return true;
				case ERROR_MORE_DATA: {
					if (size % sizeof(char)) {
						return false;
					}
					buffer.resize(size / sizeof(char), '\0');
				} break;
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