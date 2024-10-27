#pragma once

#include "Voicemeeter.UI/Policies/CircularStateChange.h"

using namespace Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				using CheckboxStateChange = CircularStateChange<int, 0, 0, 1, 1>;
			}
		}
	}
}