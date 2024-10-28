#pragma once

#include "Voicemeeter.UI/Controls/Knob.h"

#include "../Graphics/Glyphs/Knob.h"

using namespace ::Voicemeeter::UI::Controls;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Controls {
				using Knob = UI::Controls::Knob<D2D::Graphics::Glyphs::Knob>;;
			}
		}
	}
}