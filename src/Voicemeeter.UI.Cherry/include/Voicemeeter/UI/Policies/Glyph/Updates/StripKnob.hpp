#ifndef VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_STRIPKNOB_HPP
#define VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_STRIPKNOB_HPP

#include <cmath>
#include <string>
#include <valarray>

#include "Voicemeeter/UI/States/StripKnob.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Glyph {
				namespace Updates {
					template<typename TPalette, typename TKnob>
					class StripKnob {
					public:
						inline explicit StripKnob(TPalette &palette)
							: _palette{ palette } {

						};
						StripKnob() = delete;
						StripKnob(StripKnob const &) = delete;
						inline StripKnob(StripKnob &&) = default;

						inline ~StripKnob() = default;

						StripKnob & operator=(StripKnob const &) = delete;
						StripKnob & operator=(StripKnob &&) = delete;

						inline void operator()(TKnob &glyph, States::StripKnob const &state) const {
							int maxLevel{ state.level.max() };
							::std::valarray<int> const &rgba{
								state.toggle
									? _palette.get_Theme()
										.Warning
									: state.hold
										? 0 < state.degree
											? _palette.get_Theme()
												.Error
											: _palette.get_Theme()
												.Ok
										: 700 < maxLevel
											? 1000 < maxLevel
												? _palette.get_Theme()
													.EqHigh
												: _palette.get_Theme()
													.EqNormal
											: 5 < maxLevel
												? _palette.get_Theme()
													.EqLow
												: _palette.get_Theme()
													.Inactive
							};
							glyph.set_Color(rgba);
							glyph.set_Angle(state.degree);
							glyph.set_Label(state.hold
								? ::std::to_string(::std::abs(state.degree)
									/ (45 * SCALING_FACTOR / 12))
								: state.label);
							glyph.set_LabelColor(rgba);
						};

					private:
						TPalette &_palette;
					};
				}
			}
		}
	}
}

#endif
