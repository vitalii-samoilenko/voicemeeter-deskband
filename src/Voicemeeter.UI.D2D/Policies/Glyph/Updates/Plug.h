#pragma once

#include <type_traits>

#include "../../../Graphics/Bundles/Plug.h"
#include "Passthrough.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						template<typename TBundle, typename TGlyph,
							::std::enable_if_t<
								::std::is_base_of_v<Graphics::Bundles::Plug, TBundle>,
								bool> = true>
						using Plug = Passthrough<TBundle, TGlyph, int>;
					}
				}
			}
		}
	}
}