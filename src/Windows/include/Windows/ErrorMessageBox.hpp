#ifndef WINDOWS_ERRORMESSAGEBOX_HPP
#define WINDOWS_ERRORMESSAGEBOX_HPP

#include "memory.hpp"

#include <windows.h>

namespace Windows {
	inline int ErrorMessageBox(HMODULE hModule, DWORD cptCode, DWORD errCode) {
		LPWSTR lpCptBuff{ NULL };
		LPWSTR lpErrBuff{ NULL };
		auto guardBuff = ::estd::make_guard([&lpCptBuff, &lpErrBuff]()->void {
			::LocalFree(lpCptBuff);
			::LocalFree(lpErrBuff);
		});
		::FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS
			| FORMAT_MESSAGE_FROM_HMODULE,
			hModule,
			cptCode,
			0,
			reinterpret_cast<LPWSTR>(&lpCptBuff),
			0,
			NULL
		);
		::FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS
			| FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_FROM_SYSTEM,
			hModule,
			errCode,
			0,
			reinterpret_cast<LPWSTR>(&lpErrBuff),
			0,
			NULL
		);
		return ::MessageBoxW(NULL, lpErrBuff, lpCptBuff,
			MB_OK | MB_ICONERROR | MB_SERVICE_NOTIFICATION
		);
	};
}

#endif
