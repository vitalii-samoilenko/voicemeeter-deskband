#include <vector>

#include "Registry.h"

using namespace ::Windows::Registry;

template<>
bool ::Windows::Registry::TryGetValue(HKEY hKey, const ::std::wstring& subKey, const ::std::wstring& valueName, ::std::wstring& value) {
	::std::vector<wchar_t> buffer{ L'\0' };
	DWORD flag{ RRF_RT_REG_SZ };
	DWORD size{ sizeof(wchar_t) };
	for (;;) {
		switch (RegGetValueW(hKey, subKey.c_str(), valueName.c_str(), flag, NULL, buffer.data(), &size)) {
		case ERROR_SUCCESS: {
			value.assign(buffer.data());
		} return true;
		case ERROR_MORE_DATA: {
			buffer.resize(size / sizeof(wchar_t), L'\0');
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