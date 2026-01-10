#ifndef VOICEMEETER_UI_ADAPTERS_INTERACTIVITY_CONTROLLERS_STRIPKNOB_HPP
#define VOICEMEETER_UI_ADAPTERS_INTERACTIVITY_CONTROLLERS_STRIPKNOB_HPP

#include <utility>

#include "Voicemeeter/UI/States/StripKnob.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			namespace Interactivity {
				namespace Controllers {
					template<typename TStripKnob>
					class StripKnob : public TStripKnob {
					public:
						template<typename... Args>
						inline explicit StripKnob(Args &&...args)
							: TStripKnob{ ::std::forward<Args>(args)... } {

						};
						StripKnob() = delete;
						StripKnob(StripKnob const &) = delete;
						StripKnob(StripKnob &&) = delete;

						inline ~StripKnob() = default;

						StripKnob & operator=(StripKnob const &) = delete;
						StripKnob & operator=(StripKnob &&) = delete;

						inline void set_DefaultState() {
							States::StripKnob state{ TStripKnob::get_State() };
							state.degree = DEFAULT;
							state.hold = true;
							TStripKnob::set_State(state);
						};
						inline void set_HoldState(bool value) {
							States::StripKnob state{ TStripKnob::get_State() };
							state.hold = value;
							TStripKnob::set_State(state);
						};
						inline void toggle_MuteState() {
							States::StripKnob state{ TStripKnob::get_State() };
							state.toggle = !state.toggle;
							TStripKnob::set_State(state);
						};
						inline void add_GainState(int value) {
							States::StripKnob state{ TStripKnob::get_State() };
							if (state.degree < MIN - value) {
								state.degree = MIN;
							} else if (MAX - value < state.degree) {
								state.degree = MAX;
							} else {
								state.degree += value;
							}
							state.hold = true;
							TStripKnob::set_State(state);
						};

					private:
						static constexpr int DEFAULT{ 0 };
						static constexpr int MIN{ -225 * SCALING_FACTOR };
						static constexpr int MAX{ 45 * SCALING_FACTOR };
					};
				}
			}
		}
	}
}

#endif
