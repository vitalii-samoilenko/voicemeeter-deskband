#pragma once

#include <type_traits>

#include "Voicemeeter.UI/Decorators/Interactivity/Checkbox.h"

#include "../../Controls/Vban.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Decorators {
				namespace Interactivity {
					template<
						typename TGlyph,
						typename TChangeNotify,
						typename TGlyphUpdate,
						typename TVban,
						::std::enable_if_t<
							::std::is_base_of_v<Controls::Vban<TGlyph, TChangeNotify, TGlyphUpdate>, TVban>,
							bool> = true>
					using Vban = UI::Decorators::Interactivity::Checkbox<
						TGlyph,
						TChangeNotify,
						TGlyphUpdate,
						TVban>;
				}
			}
		}
	}
}