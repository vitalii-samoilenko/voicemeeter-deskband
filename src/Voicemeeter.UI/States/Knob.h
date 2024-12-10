#pragma once

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace States {
			struct Knob {
				size_t id;
				int gain;
				::std::valarray<int> level;
				bool toggle;
				bool hold;
			};
		}
	}
}