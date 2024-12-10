#pragma once

#include "Voicemeeter.UI/Controls/Checkbox.h"

#include "../Policies/Glyph/Updates/Plug.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Controls {
				template<
					typename TBundle,
					typename TGlyph,
					typename TChangeNotify>
				using Plug = UI::Controls::Checkbox<
					TGlyph,
					TChangeNotify,
					Policies::Glyph::Updates::Plug<TBundle, TGlyph>>;
			}
		}
	}
}