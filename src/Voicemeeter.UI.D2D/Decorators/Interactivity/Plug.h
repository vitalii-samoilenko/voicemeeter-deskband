#pragma once

#include "Voicemeeter.UI/Decorators/Interactivity/Checkbox.h"

#include "../../Controls/Plug.h"
#include "../../Policies/Glyph/Updates/Plug.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				namespace Interactivity {
					template<
						typename TBundle,
						typename TGlyph,
						typename TChangeNotify>
					using Plug = UI::Decorators::Interactivity::Checkbox<
						TGlyph,
						TChangeNotify,
						Policies::Glyph::Updates::Plug<TBundle, TGlyph>,
						Controls::Plug<
							TBundle,
							TGlyph,
							TChangeNotify>>;
				}
			}
		}
	}
}