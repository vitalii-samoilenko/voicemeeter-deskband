#pragma once

#include <type_traits>

#include "../../../../Decorators/Bundle/Animations/Vban.h"
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
							struct Vban {
								static_assert(
									::std::is_base_of_v<Decorators::Bundle::Animations::Vban<TInstrumentation, TBundle>, TAnimation>,
									"TAnimation must be derived from Vban");
								static_assert(
									::std::is_base_of_v<Graphics::Glyph<TInstrumentation, TAnimation>, TGlyph>,
									"TGlyph must be derived from Glyph");

								inline void operator()(TGlyph& glyph, const int& state) const {
									using Decorators::Bundle::Animations::Vban<TInstrumentation, TBundle>;

									glyph.get_Bundle()
										.get_Velocity()
											[active] = state
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