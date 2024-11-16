#pragma once

#include "Voicemeeter.UI/Decorators/Interactivity/Checkbox.h"

#include "../../Graphics/Glyphs/Vban.h"

using namespace Voicemeeter::UI::Decorators::Interactivity;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				namespace Interactivity {
					template<typename TCheckbox>
					using Vban = UI::Decorators::Interactivity::Checkbox<TCheckbox, Graphics::Glyphs::Vban>;
				}
			}
		}
	}
}