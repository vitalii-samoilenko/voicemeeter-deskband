#pragma once

namespace Voicemeeter {
	namespace UI {
		namespace States {
			struct Knob {
				int level;
				bool pinned;

				bool operator==(const Knob& rhs) const {
					return level == rhs.level
						&& pinned == rhs.pinned;
				}
			};
		}
	}
}