#pragma once

#include "Voicemeeter.UI/Decorators/Interactivity/Checkbox.h"

#include "../../Controls/VBan.h"
#include "../../Policies/Glyph/Updates/Vban.h"

using namespace Voicemeeter::UI::Decorators::Interactivity;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				namespace Interactivity {
					template<
						typename TGlyph,
						typename TChangeNotify>
					using Vban = UI::Decorators::Interactivity::Checkbox<
						TGlyph,
						TChangeNotify,
						Policies::Glyph::Updates::Vban<TGlyph>,
						Controls::Vban<
							TGlyph,
							TChangeNotify>>;
				}
			}
		}
	}
}