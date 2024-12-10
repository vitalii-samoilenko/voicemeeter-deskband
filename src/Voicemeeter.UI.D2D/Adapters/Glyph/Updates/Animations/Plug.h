#pragma once

#include <type_traits>

#include "../../../../Decorators/Bundle/Animations/Plug.h"
#include "../../../../Graphics/Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						namespace Animations {
							template<typename TBundle, typename TAnimation, typename TGlyph>
							class Plug : public TGlyph {
								static_assert(
									::std::is_base_of_v<Decorators::Bundle::Animations::Plug<TBundle>, TAnimation>,
									"TAnimation must be derived from Plug");
								static_assert(
									::std::is_base_of_v<Graphics::Glyph<TAnimation>, TGlyph>,
									"TGlyph must be derived from Glyph");

								enum animation_vector : size_t {
									active = 0
								};

							public:
								using TGlyph::TGlyph;

								Plug() = delete;
								Plug(const Plug&) = delete;
								Plug(Plug&&) = delete;

								~Plug() = default;

								Plug& operator=(const Plug&) = delete;
								Plug& operator=(Plug&&) = delete;

								inline void Update(const int& state) {
									TGlyph::get_Bundle()
										.get_Velocity()
											[active] = state
												? 1LL
												: -1LL;
									TGlyph::get_Bundle()
										.get_Palette()
											.Queue(TGlyph::get_Bundle());
								};
							};
						}
					}
				}
			}
		}
	}
}