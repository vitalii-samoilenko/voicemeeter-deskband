#pragma once

#include <type_traits>

#include "Voicemeeter.UI/States/Knob.h"

#include "../../../../Decorators/Bundle/Animations/Knob.h"
#include "../../../../Graphics/Glyph.h"
#include "../../../../Policies/State/Changes/Knob.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						namespace Animations {
							template<typename TBundle, typename TAnimation, typename TGlyph>
							class Knob : public TGlyph {
								static_assert(
									::std::is_base_of_v<Decorators::Bundle::Animations::Knob<TBundle>, TAnimation>,
									"TAnimation must be derived from Knob");
								static_assert(
									::std::is_base_of_v<Graphics::Glyph<TAnimation>, TGlyph>,
									"TGlyph must be derived from Glyph");

								enum animation_vector : size_t {
									level_low = 0, level_medium = 1, level_high = 2,
									gain_danger = 3,
									hold = 4, toggle = 5,
									label = 6
								};

							public:
								using TGlyph::TGlyph;

								Knob() = delete;
								Knob(const Knob&) = delete;
								Knob(Knob&&) = delete;

								~Knob() = default;

								Knob& operator=(const Knob&) = delete;
								Knob& operator=(Knob&&) = delete;

								inline void Update(const States::Knob& state) {
									::std::valarray<long long>& vertex{ 
										TGlyph::get_Bundle()
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
									TGlyph::get_Bundle()
										.set_To(state.hold
											? Policies::State::Changes::Knob::ToLabel(state.gain)
											: Policies::State::Changes::Knob::ToLabel(state.id));
									TGlyph::get_Bundle()
										.set_Angle(state.gain);
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