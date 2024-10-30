#pragma once

#include "Voicemeeter.UI/Policies/KnobInteractivity.h"

#include "../Graphics/Glyphs/Knob.h"

using namespace Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				using KnobInteractivity = KnobInteractivity<Graphics::Glyphs::Knob>;
			}
		}
	}
}