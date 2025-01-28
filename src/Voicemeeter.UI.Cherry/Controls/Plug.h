#pragma once

#include "Voicemeeter.UI/Controls/Checkbox.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Controls {
				template<
					typename TGlyph,
					typename TChangeNotify,
					typename TGlyphUpdate>
				using Plug = UI::Controls::Checkbox<
					TGlyph,
					TChangeNotify,
					TGlyphUpdate>;
			}
		}
	}
}