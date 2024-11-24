#pragma once

#include "Voicemeeter.UI/Controls/Checkbox.h"

#include "../Graphics/Glyphs/Plug.h"
#include "../Policies/Glyph/Updates/Plug.h"

using namespace ::Voicemeeter::UI::Controls;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Controls {
				template<
					typename TGlyph,
					typename TChangeNotify>
				using Plug = UI::Controls::Checkbox<
					TGlyph,
					TChangeNotify,
					Policies::Glyph::Updates::Plug<TGlyph>>;
			}
		}
	}
}