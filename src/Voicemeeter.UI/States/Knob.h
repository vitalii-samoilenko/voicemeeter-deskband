#pragma once

namespace Voicemeeter {
	namespace UI {
		namespace States {
			struct Knob {
				int gain;
				int level;
				bool enabled;
				bool pinned;

				bool operator==(const Knob& rhs) const {
					return gain == rhs.gain
						&& level == rhs.level
						&& enabled == rhs.enabled
						&& pinned == rhs.pinned;
				}

				bool operator<(const Knob& rhs) const {
					return gain < rhs.gain;
				}
			};
		}
	}
}