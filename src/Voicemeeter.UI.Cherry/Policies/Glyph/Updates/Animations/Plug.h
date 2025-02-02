#pragma once

#include <type_traits>

#include "../../../../Decorators/Bundle/Animations/Plug.h"
#include "../../../../Graphics/Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						namespace Animations {
							template<
								typename TInstrumentation,
								typename TBundle,
								typename TAnimation,
								typename TGlyph>
							struct Plug {
								static_assert(
									::std::is_base_of_v<Decorators::Bundle::Animations::Plug<TInstrumentation, TBundle>, TAnimation>,
									"TAnimation must be derived from Plug");
								static_assert(
									::std::is_base_of_v<Graphics::Glyph<TInstrumentation, TAnimation>, TGlyph>,
									"TGlyph must be derived from Glyph");

								inline void operator()(TGlyph& glyph, const int& state) const {
									using Animation = typename TAnimation::animation_vector;

									glyph.get_Bundle()
										.get_Velocity()
											[Animation::active] = state
												? 1LL
												: -1LL;
									glyph.get_Bundle()
										.get_Palette()
											.get_Queue() += glyph.get_Bundle();
								};
							};
						}
					}
				}
			}
		}
	}
}