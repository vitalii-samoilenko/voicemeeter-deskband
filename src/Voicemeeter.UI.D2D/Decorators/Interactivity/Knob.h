#pragma once

#include "Voicemeeter.UI/Decorators/Interactivity/Knob.h"

#include "../../Graphics/Glyphs/Knob.h"

using namespace Voicemeeter::UI::Decorators::Interactivity;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				namespace Interactivity {
					template<UI::Direction Direction, typename TKnob>
					using Knob = UI::Decorators::Interactivity::Knob<Direction, TKnob, Graphics::Glyphs::Knob>;
				}
			}
		}
	}
}