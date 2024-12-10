#pragma once

#include "Voicemeeter.UI/Decorators/Interactivity/Checkbox.h"

#include "../../Controls/VBan.h"
#include "../../Policies/Glyph/Updates/Vban.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				namespace Interactivity {
					template<
						typename TBundle,
						typename TGlyph,
						typename TChangeNotify>
					using Vban = UI::Decorators::Interactivity::Checkbox<
						TGlyph,
						TChangeNotify,
						Policies::Glyph::Updates::Vban<TBundle, TGlyph>,
						Controls::Vban<
							TBundle,
							TGlyph,
							TChangeNotify>>;
				}
			}
		}
	}
}