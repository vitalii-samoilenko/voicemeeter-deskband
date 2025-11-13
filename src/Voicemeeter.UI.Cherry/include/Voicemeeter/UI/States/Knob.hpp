#ifndef VOICEMEETER_UI_STATES_KNOB_HPP
#define VOICEMEETER_UI_STATES_KNOB_HPP

#include <string>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace States {
			struct Knob {
				::std::string label;
				int gain;
				::std::valarray<int> level;
				bool toggle;
				bool hold;
			};
		}
	}
}

#endif
