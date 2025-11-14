#ifndef VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_ANIMATED_STRIPKNOB_HPP
#define VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_ANIMATED_STRIPKNOB_HPP

#include <string>
#include <valarray>

#include "Voicemeeter/UI/States/StripKnob.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Glyph {
				namespace Updates {
					namespace Animated {
						template<typename TKnob>
						struct StripKnob {
							inline void operator()(TKnob &glyph, States::StripKnob const &state) const {
								enum state { inactive, low, normal, high };
								int maxLevel{ state.level.max() };
								state eq{
									700 < maxLevel
										? 1000 < maxLevel
											? high
											: normal
										: 5 < maxLevel
											? low
											: inactive
								};
								state gain{
									state.degree < 0
										? high
										: normal
								};
								glyph.set_Velocity(::std::valarray<long long> {
									normalize(eq == low), normalize(eq == normal), normalize(eq == high),
									normalize(state.hold),
									normalize(gain == high),
									normalize(state.mute)
								});
								glyph.set_ToLabel(state.hold
									? ::std::to_string(::std::abs(state.degree)
										/ (45 * SCALING_FACTOR / 12))
									: state.label);
								glyph.set_Angle(state.degree);
							};

						private:
							static long long normalize(bool value) {
								return value ? 1 : -1;
							};
						};

						template<typename TPalette, typename TKnob>
						class StripKnobFrame {
						public:
							inline explicit StripKnobFrame(TPalette &palette)
								: _palette{ palette } {

							};
							StripKnobFrame() = delete;
							StripKnobFrame(StripKnobFrame const &) = delete;
							inline StripKnobFrame(StripKnobFrame &&) = default;

							inline ~StripKnobFrame() = default;

							StripKnobFrame & operator=(StripKnobFrame const &) = delete;
							StripKnobFrame & operator=(StripKnobFrame &&) = delete;

							inline void operator()(TKnob &glyph) const {
								enum dimension : size_t {
									/* eqInactive */ eqLow = 0, eqNormal = 1, eqHigh = 2,
									hold = 3,
									/* gainNormal */ gainHigh = 4,
									mute = 5
									/* warning */
								};
							};

						private:
							TPalette &_palette;
						};
					}
				}
			}
		}
	}
}

#endif
