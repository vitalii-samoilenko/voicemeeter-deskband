#pragma once

#include <type_traits>

#include "../../../Graphics/Glyphs/Plug.h"
#include "Passthrough.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						template<typename TPlug,
							::std::enable_if_t<
								::std::is_base_of_v<Graphics::Glyphs::Plug, TPlug>,
								bool> = true>
						using Plug = Passthrough<TPlug, int>;
					}
				}
			}
		}
	}
}