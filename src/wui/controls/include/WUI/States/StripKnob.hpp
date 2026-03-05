#ifndef WUI_STATES_STRIPKNOB_HPP
#define WUI_STATES_STRIPKNOB_HPP

#include "math.hpp"

namespace WUI {
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

#endif
