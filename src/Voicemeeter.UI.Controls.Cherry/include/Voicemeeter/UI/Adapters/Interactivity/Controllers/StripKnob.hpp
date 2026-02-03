#ifndef VOICEMEETER_UI_ADAPTERS_INTERACTIVITY_CONTROLLERS_STRIPKNOB_HPP
#define VOICEMEETER_UI_ADAPTERS_INTERACTIVITY_CONTROLLERS_STRIPKNOB_HPP

#include <utility>

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			namespace Interactivity {
				namespace Controllers {
					template<typename TStripKnob>
					class StripKnob : public TStripKnob {
					public:
						using state_t = typename TStripKnob::state_t;

						template<typename ...Args>
						inline explicit StripKnob(Args &&...args)
							: TStripKnob{ ::std::forward<Args>(args) ... } {

						};
						StripKnob() = delete;
						StripKnob(StripKnob const &) = delete;
						StripKnob(StripKnob &&) = delete;

						inline ~StripKnob() = default;

						StripKnob & operator=(StripKnob const &) = delete;
						StripKnob & operator=(StripKnob &&) = delete;

						inline void set_DefaultState() {
							state_t state{ TStripKnob::get_State() };
							state.degree = Default;
							state.hold = true;
							TStripKnob::set_State(state);
						};
						inline void set_HoldState(bool value) {
							state_t state{ TStripKnob::get_State() };
							state.hold = value;
							TStripKnob::set_State(state);
						};
						inline void toggle_MuteState() {
							state_t state{ TStripKnob::get_State() };
							state.toggle = !state.toggle;
							TStripKnob::set_State(state);
						};
						inline void add_GainState(num_t value) {
							constexpr num_t Default{ 0 };
							constexpr num_t Min{ push(-225) };
							constexpr num_t Max{ push(45) };
							state_t state{ TStripKnob::get_State() };
							if (state.degree < Min - value) {
								state.degree = Min;
							} else if (Max - value < state.degree) {
								state.degree = Max;
							} else {
								state.degree += value;
							}
							state.hold = true;
							TStripKnob::set_State(state);
						};
					};
				}
			}
		}
	}
}

#endif
