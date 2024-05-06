#pragma once

#include <windows.h>

#include "messages.h"

int ErrorMessageBox(
	DWORD cptCode,
	DWORD errCode
);

inline int ErrorMessageBox(
	DWORD errCode
) {
	return ErrorMessageBox(CPT_ERROR, errCode);
}