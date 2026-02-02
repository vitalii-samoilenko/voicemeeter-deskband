#ifndef VOICEMEETER_UI_STATES_PLUG_HPP
#define VOICEMEETER_UI_STATES_PLUG_HPP

namespace Voicemeeter {
	namespace UI {
		namespace States {
			struct Plug {
				size_t from;
				size_t to;
				bool toggle;
			};
		}
	}
}

#endif
