#ifndef VOICEMEETER_UI_STATES_STRIPKNOB_HPP
#define VOICEMEETER_UI_STATES_STRIPKNOB_HPP

namespace Voicemeeter {
	namespace UI {
		namespace States {
			struct StripKnob {
				int degree;
				bool toggle;
				bool hold;
				int level;
			};
		}
	}
}

#endif
