#pragma once

#include "Voicemeeter.UI/Controls/Checkbox.h"

#include "../Adapters/Glyph/IUpdate.h"
#include "../Graphics/Glyphs/Plug.h"

using namespace ::Voicemeeter::UI::Controls;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Controls {
				using Plug = UI::Controls::Checkbox<Adapters::Glyph::IUpdate<D2D::Graphics::Glyphs::Plug, int>>;
			}
		}
	}
}