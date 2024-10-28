#pragma once

#include "Voicemeeter.UI/Policies/KnobStateChange.h"

using namespace Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				using KnobStateChange = UI::Policies::KnobStateChange<9000, -13500, 13500, 1>;
			}
		}
	}
}