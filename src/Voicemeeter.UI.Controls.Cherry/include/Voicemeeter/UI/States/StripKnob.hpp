#ifndef VOICEMEETER_UI_STATES_STRIPKNOB_HPP
#define VOICEMEETER_UI_STATES_STRIPKNOB_HPP

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace States {
			struct StripKnob {
				num_t degree;
				bool toggle;
				bool hold;
				num_t level;
			};
		}
	}
}

#endif
