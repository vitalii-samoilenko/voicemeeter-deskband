#pragma once

#include "Voicemeeter.UI/Decorators/Interactivity/Slider.h"

#include "../../Graphics/Glyphs/Gainer.h"

using namespace Voicemeeter::UI::Decorators::Interactivity;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				namespace Interactivity {
					template<UI::Direction Direction, typename TGainer>
					using Gainer = UI::Decorators::Interactivity::Slider<Direction, TGainer, Graphics::Glyphs::Gainer>;
				}
			}
		}
	}
}