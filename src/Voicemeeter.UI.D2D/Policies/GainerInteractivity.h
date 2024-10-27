#pragma once

#include "Voicemeeter.UI/Policies/SliderInteractivity.h"

#include "../Graphics/Glyphs/Gainer.h"

using namespace ::Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				using GainerInteractivity = SliderInteractivity<D2D::Graphics::Glyphs::Gainer>;
			}
		}
	}
}