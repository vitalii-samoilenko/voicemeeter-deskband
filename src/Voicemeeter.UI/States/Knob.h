#pragma once

namespace Voicemeeter {
	namespace UI {
		namespace States {
			struct Knob {
				int gain;
				int leftLevel;
				int rightLevel;
				bool enabled;
				bool pinned;

				bool operator==(const Knob& rhs) const {
					return gain == rhs.gain
						&& leftLevel == rhs.leftLevel
						&& rightLevel == rhs.rightLevel
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