#pragma once

#include "Voicemeeter.DeskBand.UI/Policies/CircularStateChange.h"

using namespace Voicemeeter::DeskBand::UI::Policies;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace D2D {
				namespace Policies {
					using CheckboxStateChange = CircularStateChange<int, 0, 0, 1, 1>;
				}
			}
		}
	}
}