#pragma once

#include <type_traits>

#include "Voicemeeter.UI/States/Knob.h"

#include "../../../Graphics/Glyphs/Knob.h"
#include "Passthrough.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						template<typename TKnob,
							::std::enable_if_t<
								::std::is_base_of_v<Graphics::Glyphs::Knob, TKnob>,
								bool> = true>
						using Knob = Passthrough<TKnob, States::Knob>;
					}
				}
			}
		}
	}
}