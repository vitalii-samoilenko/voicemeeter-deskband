#pragma once

#include "Voicemeeter.UI/Controls/Slider.h"

#include "../Graphics/Glyphs/Gainer.h"

using namespace ::Voicemeeter::UI::Controls;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Controls {
				using Gainer = Slider<D2D::Graphics::Glyphs::Gainer>;
			}
		}
	}
}