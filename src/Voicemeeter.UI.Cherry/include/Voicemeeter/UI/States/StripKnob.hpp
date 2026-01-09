#ifndef VOICEMEETER_UI_STATES_STRIPKNOB_HPP
#define VOICEMEETER_UI_STATES_STRIPKNOB_HPP

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace States {
			struct StripKnob {
				int degree;
				bool toggle;
				bool hold;
				::std::valarray<int> level;
			};
		}
	}
}

#endif
