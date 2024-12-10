#pragma once

#include <type_traits>

#include "Voicemeeter.UI/States/Knob.h"

#include "../../../Graphics/Bundles/Knob.h"
#include "Passthrough.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						template<typename TBundle, typename TGlyph,
							::std::enable_if_t<
								::std::is_base_of_v<Graphics::Bundles::Knob, TBundle>,
								bool> = true>
						using Knob = Passthrough<TBundle, TGlyph, States::Knob>;
					}
				}
			}
		}
	}
}