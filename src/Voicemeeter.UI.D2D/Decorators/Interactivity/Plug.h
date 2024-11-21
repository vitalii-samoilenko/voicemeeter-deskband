#pragma once

#include "Voicemeeter.UI/Decorators/Interactivity/Checkbox.h"

#include "../../Adapters/Glyph/IUpdate.h"
#include "../../Graphics/Glyphs/Plug.h"

using namespace Voicemeeter::UI::Decorators::Interactivity;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				namespace Interactivity {
					template<typename TCheckbox>
					using Plug = UI::Decorators::Interactivity::Checkbox<TCheckbox, Adapters::Glyph::IUpdate<Graphics::Glyphs::Plug, int>>;
				}
			}
		}
	}
}