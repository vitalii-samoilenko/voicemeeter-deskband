#pragma once

#include "Voicemeeter.UI/Controls/Checkbox.h"

#include "../Adapters/Glyph/IUpdate.h"
#include "../Graphics/Glyphs/Vban.h"

using namespace ::Voicemeeter::UI::Controls;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Controls {
				using Vban = UI::Controls::Checkbox<Adapters::Glyph::IUpdate<D2D::Graphics::Glyphs::Vban, int>>;
			}
		}
	}
}