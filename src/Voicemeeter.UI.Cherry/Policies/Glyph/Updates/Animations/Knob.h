#pragma once

#include <type_traits>

#include "../../../../Decorators/Bundle/Animations/Knob.h"
#include "../../../../Graphics/Glyph.h"
#include "../../../../Policies/State/Changes/Knob.h"
#include "../../../../States/Knob.h"

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
							struct Knob {
								static_assert(
									::std::is_base_of_v<Decorators::Bundle::Animations::Knob<TInstrumentation, TBundle>, TAnimation>,
									"TAnimation must be derived from Knob");
								static_assert(
									::std::is_base_of_v<Graphics::Glyph<TInstrumentation, TAnimation>, TGlyph>,
									"TGlyph must be derived from Glyph");

								inline void operator()(TGlyph& glyph, const States::Knob& state) const {
									using Decorators::Bundle::Animations::Knob<TInstrumentation, TBundle>;

									::std::valarray<long long>& vertex{ 
										glyph.get_Bundle()
											.get_Velocity()
									};
									if (state.hold) {
										vertex[hold] = 1;
										vertex[label] = 1;
									} else {
										vertex[hold] = -1;
										vertex[label] = -1;
									}
									vertex[toggle] = state.toggle
										? 1
										: -1;
									vertex[gain_danger] = Policies::State::Changes::Knob::DEFAULT < state.gain
										? 1
										: -1;
									int level{
										(state.level.size()
											? state.level.max()
											: 0)
									};
									if (level < 5) {
										vertex[level_low] = -1;
										vertex[level_medium] = -1;
										vertex[level_high] = -1;
									} else if (level < 700) {
										vertex[level_low] = 1;
										vertex[level_medium] = -1;
										vertex[level_high] = -1;
									} else if (level < 10000) {
										vertex[level_low] = 1;
										vertex[level_medium] = 1;
										vertex[level_high] = -1;
									} else {
										vertex[level_low] = 1;
										vertex[level_medium] = 1;
										vertex[level_high] = 1;
									}
									glyph.get_Bundle()
										.set_To(state.hold
											? Policies::State::Changes::Knob::ToLabel(state.gain)
											: Policies::State::Changes::Knob::ToLabel(state.id));
									glyph.get_Bundle()
										.set_Angle(state.gain);
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