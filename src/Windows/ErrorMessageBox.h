#pragma once

#include <windows.h>

#include "Messages.h"

namespace Windows {
	int ErrorMessageBox(
		HMODULE hModule,
		long long cptCode,
		long long errCode
	);

	inline int ErrorMessageBox(
		HMODULE hModule,
		long long errCode
	) {
		return ErrorMessageBox(hModule, CPT_ERROR, errCode);
	}

	inline int ErrorMessageBox(
		long long errCode
	) {
		return ErrorMessageBox(NULL, CPT_ERROR, errCode);
	}
}