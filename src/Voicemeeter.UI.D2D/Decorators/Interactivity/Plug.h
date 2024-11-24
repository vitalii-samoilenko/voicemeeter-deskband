#pragma once

#include "Voicemeeter.UI/Decorators/Interactivity/Checkbox.h"

#include "../../Controls/Plug.h"
#include "../../Policies/Glyph/Updates/Plug.h"

using namespace Voicemeeter::UI::Decorators::Interactivity;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				namespace Interactivity {
					template<
						typename TGlyph,
						typename TChangeNotify>
					using Plug = UI::Decorators::Interactivity::Checkbox<
						Controls::Plug<
							TGlyph,
							TChangeNotify>,
						TGlyph,
						TChangeNotify,
						Policies::Glyph::Updates::Plug<TGlyph>>;
				}
			}
		}
	}
}