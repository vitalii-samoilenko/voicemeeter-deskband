#pragma once

#include "Voicemeeter.DeskBand.UI/Controls/Slider.h"

#include "../Graphics/Glyphs/Gainer.h"

using namespace Voicemeeter::DeskBand::UI::Controls;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace D2D {
				namespace Controls {
					using Gainer = Slider<D2D::Graphics::Glyphs::Gainer>;
				}
			}
		}
	}
}