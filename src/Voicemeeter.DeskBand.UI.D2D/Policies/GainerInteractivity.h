#pragma once

#include "Voicemeeter.DeskBand.UI/Policies/SliderInteractivity.h"

#include "../Graphics/Glyphs/Gainer.h"

using namespace ::Voicemeeter::DeskBand::UI::Policies;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace D2D {
				namespace Policies {
					using GainerInteractivity = SliderInteractivity<D2D::Graphics::Glyphs::Gainer>;
				}
			}
		}
	}
}