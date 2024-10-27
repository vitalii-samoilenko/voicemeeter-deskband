#pragma once

#include <windows.h>

#include "Messages.h"

namespace Windows {
	int ErrorMessageBox(
		long long cptCode,
		long long errCode
	);

	inline int ErrorMessageBox(
		long long errCode
	) {
		return ErrorMessageBox(CPT_ERROR, errCode);
	}
}