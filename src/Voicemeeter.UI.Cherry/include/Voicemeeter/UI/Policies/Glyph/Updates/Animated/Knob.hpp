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
						template<typename TPalette>
						class StripKnob {
						public:
							inline explicit StripKnob(TPalette &palette)
								: _palette{ palette }
								, _vertex{
									0, 0, 0,
									0,
									0, 0,
									0
								}
								, _velocity{
									0, 0, 0,
									0,
									0, 0,
									0
								} {

							};
							StripKnob() = delete;
							StripKnob(StripKnob const &) = delete;
							inline StripKnob(StripKnob &&) = default;

							inline ~StripKnob() = default;

							StripKnob & operator=(StripKnob const &) = delete;
							StripKnob & operator=(StripKnob &&) = delete;

							template<typename TKnob>
							inline void operator()(TKnob &glyph, States::StripKnob const &state) {

							};

						private:
							enum dimension : size_t {
								eqLow = 0, eqNormal = 1, eqHigh = 2,
								hold = 3,
								gainNormal = 4, gainHigh = 5,
								mute = 6
							};

							class Tick {
							};

							TPalette &_palette;
							::std::valarray<long long> _vertex;
							::std::valarray<long long> _velocity;
						};
					}
				}
			}
		}
	}
}

#endif
