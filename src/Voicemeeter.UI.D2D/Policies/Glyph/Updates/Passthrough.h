#pragma once

#include <type_traits>

#include "../../../Graphics/Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						template<typename TGlyph, typename TState>
						struct Passthrough {
							static_assert(
								::std::is_base_of_v<Graphics::Glyph, TGlyph>,
								"TGlyph must be derived from Glyph");

							inline void operator()(TGlyph& glyph, const TState& state) const {
								glyph.Update(state);
							}
						};
					}
				}
			}
		}
	}
}