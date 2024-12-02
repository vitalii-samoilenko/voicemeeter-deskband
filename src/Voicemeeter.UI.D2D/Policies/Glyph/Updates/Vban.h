#pragma once

#include <type_traits>

#include "../../../Graphics/Glyphs/Vban.h"
#include "Passthrough.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						template<typename TVban,
							::std::enable_if_t<
								::std::is_base_of_v<Graphics::Glyphs::Vban, TVban>,
								bool> = true>
						using Vban = Passthrough<UI::Policies::Size::Scales::Stretch, TVban, int>;
					}
				}
			}
		}
	}
}