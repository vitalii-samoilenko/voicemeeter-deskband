#pragma once

#include "Voicemeeter.UI/Policies/KnobInteractivity.h"

#include "../Graphics/Glyphs/Knob.h"

using namespace Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				using RightKnobInteractivity = KnobInteractivity<UI::Direction::Right, Graphics::Glyphs::Knob>;
				using DownKnobInteractivity = KnobInteractivity<UI::Direction::Down, Graphics::Glyphs::Knob>;
			}
		}
	}
}