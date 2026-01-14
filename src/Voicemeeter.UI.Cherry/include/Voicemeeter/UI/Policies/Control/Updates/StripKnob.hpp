#ifndef VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_STRIPKNOB_HPP
#define VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_STRIPKNOB_HPP

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Control {
				namespace Updates {
					template<typename TToolkit, typename TStripKnob>
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

						inline void operator()(
							TStripKnob &control, typename TStripKnob::state_t const &state) const {
							control.set_FrameColor(
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
							);
							control.set_IndicatorAngle(state.degree);
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
