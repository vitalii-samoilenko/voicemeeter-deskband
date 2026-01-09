#ifndef VOICEMEETER_UI_DECORATORS_STRIPKNOB_HPP
#define VOICEMEETER_UI_DECORATORS_STRIPKNOB_HPP

#include <utility>

#include "Voicemeeter/UI/States/StripKnob.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
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
					state.degree += value;
					TStripKnob::set_State(state);
				};
			};
		}
	}
}

#endif
