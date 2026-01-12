#ifndef VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_STRIPKNOB_HPP
#define VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_STRIPKNOB_HPP

#include <valarray>

#include "Voicemeeter/UI/States/StripKnob.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Glyph {
				namespace Updates {
					template<typename TToolkit, typename TKnob>
					class StripKnob {
					public:
						inline explicit StripKnob(TToolkit &toolkit)
							: _toolkit{ toolkit } {

						};
						StripKnob() = delete;
						StripKnob(StripKnob const &) = delete;
						inline StripKnob(StripKnob &&) = default;

						inline ~StripKnob() = default;

						StripKnob & operator=(StripKnob const &) = delete;
						StripKnob & operator=(StripKnob &&) = delete;

						inline void operator()(TKnob &glyph, States::StripKnob const &state) const {
							::std::valarray<int> const &rgba{
								state.toggle
									? _toolkit.get_Theme()
										.Warning
									: state.hold
										? 0 < state.degree
											? _toolkit.get_Theme()
												.Error
											: _toolkit.get_Theme()
												.Ok
										: 700 < state.level
											? 1000 < state.level
												? _toolkit.get_Theme()
													.EqHigh
												: _toolkit.get_Theme()
													.EqNormal
											: 5 < state.level
												? _toolkit.get_Theme()
													.EqLow
												: _toolkit.get_Theme()
													.Inactive
							};
							glyph.set_FrameColor(rgba);
							glyph.set_IndicatorAngle(state.degree);
						};

					private:
						TToolkit &_toolkit;
					};
				}
			}
		}
	}
}

#endif
