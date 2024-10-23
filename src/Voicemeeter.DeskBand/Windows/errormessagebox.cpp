#include "estd/memory.h"

#include "errormessagebox.h"

int ::Voicemeeter::DeskBand::Windows::ErrorMessageBox(
	DWORD cptCode,
	DWORD errCode
) {
	LPWSTR lpCptBuff{ NULL };
	LPWSTR lpErrBuff{ NULL };
	auto guardBuff = ::estd::make_guard([&lpCptBuff, &lpErrBuff]()->void {
		LocalFree(lpCptBuff);
		LocalFree(lpErrBuff);
	});
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS
		| FORMAT_MESSAGE_FROM_HMODULE,
		NULL,
		cptCode,
		0,
		reinterpret_cast<LPWSTR>(&lpCptBuff),
		0,
		NULL
	);
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS
		| FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errCode,
		0,
		reinterpret_cast<LPWSTR>(&lpErrBuff),
		0,
		NULL
	);
	return MessageBoxW(NULL, lpErrBuff, lpCptBuff,
		MB_OK | MB_ICONERROR | MB_SERVICE_NOTIFICATION
	);
}