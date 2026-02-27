#ifndef VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_STRIPKNOB_HPP
#define VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_STRIPKNOB_HPP

#include "wheel.hpp"

#include "Voicemeeter/UI/Layouts/Atlas.hpp"
#include "Voicemeeter/UI/States/StripKnob.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Control {
				namespace Updates {
					template<typename TToolkit>
					class StripKnob {
					public:
						using state_t = States::StripKnob;

						inline explicit StripKnob(TToolkit &toolkit)
							: _toolkit{ toolkit } {

						};
						StripKnob() = delete;
						StripKnob(StripKnob const &) = delete;
						inline StripKnob(StripKnob &&) = default;

						inline ~StripKnob() = default;

						StripKnob & operator=(StripKnob const &) = delete;
						StripKnob & operator=(StripKnob &&) = delete;

						template<typename TStripKnob>
						inline void operator()(TStripKnob *that, state_t const &state) const {
							vector_t const *targetRgba{
								state.toggle
									? &_toolkit.get_Theme()
										.Warning
									: state.hold
										? 0 < state.degree
											? &_toolkit.get_Theme()
												.Error
											: &_toolkit.get_Theme()
												.Active
										: push(700) < state.level
											? push(10000) < state.level
												? &_toolkit.get_Theme()
													.EqHigh
												: &_toolkit.get_Theme()
													.EqMedium
											: push(5) < state.level
												? &_toolkit.get_Theme()
													.EqLow
												: &_toolkit.get_Theme()
													.Inactive
							};
							that->set_FrameColor(*targetRgba);
							that->set_LabelColor(*targetRgba);
							that->set_IndicatorAngle(state.degree);
							that->set_Label(state.hold
								? Layouts::Atlas::Label::Names
									+ pop(floor(ans(state.degree * 4 / 15)))
								: state.target);
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
