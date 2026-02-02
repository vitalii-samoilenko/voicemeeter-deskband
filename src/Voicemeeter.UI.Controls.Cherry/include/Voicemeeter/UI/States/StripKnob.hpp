#ifndef VOICEMEETER_UI_STATES_STRIPKNOB_HPP
#define VOICEMEETER_UI_STATES_STRIPKNOB_HPP

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace States {
			struct StripKnob {
				size_t target;
				num_t degree;
				bool toggle;
				bool hold;
				num_t level;
			};
		}
	}
}

#endif
