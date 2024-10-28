#pragma once

#include "CircularStateChange.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			using CheckboxStateChange = CircularStateChange<int, 0, 0, 1, 1>;
		}
	}
}