#pragma once

#include "Voicemeeter.DeskBand.UI/Policies/RangeStateChange.h"

using namespace Voicemeeter::DeskBand::UI::Policies;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace D2D {
				namespace Policies {
					using GainerStateChange = RangeStateChange<int, 11800, 2800, 13700, 1>;
				}
			}
		}
	}
}