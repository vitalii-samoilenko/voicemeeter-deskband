#pragma once

#include "Voicemeeter.UI/Controls/Knob.h"

#include "../Adapters/Glyph/IUpdate.h"
#include "../Graphics/Glyphs/Knob.h"

using namespace ::Voicemeeter::UI::Controls;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Controls {
				using Knob = UI::Controls::Knob<Adapters::Glyph::IUpdate<D2D::Graphics::Glyphs::Knob, States::Knob>>;
			}
		}
	}
}