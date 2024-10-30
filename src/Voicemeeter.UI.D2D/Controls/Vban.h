#pragma once

#include "Voicemeeter.UI/Controls/Checkbox.h"

#include "../Graphics/Glyphs/Vban.h"

using namespace ::Voicemeeter::UI::Controls;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Controls {
				using Vban = UI::Controls::Checkbox<D2D::Graphics::Glyphs::Vban>;
			}
		}
	}
}