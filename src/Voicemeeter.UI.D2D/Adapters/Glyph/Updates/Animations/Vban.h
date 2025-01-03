#pragma once

#include <type_traits>

#include "../../../../Decorators/Bundle/Animations/Vban.h"
#include "../../../../Graphics/Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						namespace Animations {
							template<typename TBundle, typename TAnimation, typename TGlyph>
							class Vban : public TGlyph {
								static_assert(
									::std::is_base_of_v<Decorators::Bundle::Animations::Vban<TBundle>, TAnimation>,
									"TAnimation must be derived from Vban");
								static_assert(
									::std::is_base_of_v<Graphics::Glyph<TAnimation>, TGlyph>,
									"TGlyph must be derived from Glyph");

								enum animation_vector : size_t {
									active = 0
								};

							public:
								using TGlyph::TGlyph;

								Vban() = delete;
								Vban(const Vban&) = delete;
								Vban(Vban&&) = delete;

								~Vban() = default;

								Vban& operator=(const Vban&) = delete;
								Vban& operator=(Vban&&) = delete;

								inline void Update(const int& state) {
									TGlyph::get_Bundle()
										.get_Velocity()
											[active] = state
												? 1LL
												: -1LL;
									TGlyph::get_Bundle()
										.get_Palette()
											.get_Queue() += TGlyph::get_Bundle();
								};
							};
						}
					}
				}
			}
		}
	}
}