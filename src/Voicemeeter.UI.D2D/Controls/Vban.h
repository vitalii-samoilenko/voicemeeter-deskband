#pragma once

#include "Voicemeeter.UI/Controls/Checkbox.h"

#include "../Graphics/Glyphs/Vban.h"
#include "../Policies/Glyph/Updates/Vban.h"

using namespace ::Voicemeeter::UI::Controls;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Controls {
				template<
					typename TGlyph,
					typename TChangeNotify>
				using Vban = UI::Controls::Checkbox<
					TGlyph,
					TChangeNotify,
					Policies::Glyph::Updates::Vban<TGlyph>>;
			}
		}
	}
}