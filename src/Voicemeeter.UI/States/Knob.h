#pragma once

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace States {
			struct Knob {
				int gain;
				::std::valarray<int> level;
				bool toggle;
				bool hold;

				bool operator==(const Knob& rhs) const {
					return gain == rhs.gain
						&& (level == rhs.level).min()
						&& toggle == rhs.toggle
						&& hold == rhs.hold;
				}

				bool operator<(const Knob& rhs) const {
					return gain < rhs.gain;
				}
			};
		}
	}
}