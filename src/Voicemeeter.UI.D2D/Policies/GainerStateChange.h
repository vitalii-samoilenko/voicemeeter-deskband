#pragma once

#include "Voicemeeter.UI/Policies/RangeStateChange.h"

using namespace Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				using GainerStateChange = RangeStateChange<int, 11800, 2800, 13700, 1>;
			}
		}
	}
}