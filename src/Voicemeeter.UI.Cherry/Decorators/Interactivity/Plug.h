#pragma once

#include <type_traits>

#include "Voicemeeter.UI/Decorators/Interactivity/Checkbox.h"

#include "../../Controls/Plug.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Decorators {
				namespace Interactivity {
					template<
						typename TGlyph,
						typename TChangeNotify,
						typename TGlyphUpdate,
						typename TPlug,
						::std::enable_if_t<
							::std::is_base_of_v<Controls::Plug<TGlyph, TChangeNotify, TGlyphUpdate>, TPlug>,
							bool> = true>
					using Plug = UI::Decorators::Interactivity::Checkbox<
						TGlyph,
						TChangeNotify,
						TGlyphUpdate,
						TPlug>;
				}
			}
		}
	}
}