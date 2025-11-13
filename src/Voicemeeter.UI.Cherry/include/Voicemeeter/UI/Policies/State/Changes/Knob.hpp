#ifndef VOICEMEETER_UI_POLICIES_STATE_CHANGES_KNOB_HPP
#define VOICEMEETER_UI_POLICIES_STATE_CHANGES_KNOB_HPP

#include <utility>

#include "Voicemeeter/UI/States/Knob.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace State {
				namespace Changes {
					struct Knob {
						inline void SetDefault(States::Knob &dst) const {
							if (dst.gain == DEFAULT) {
								return;
							}
							dst.gain = DEFAULT;
							dst.hold = true;
						};
						inline void SetNext(States::Knob &dst) const {
							if (dst.gain == MAX) {
								return;
							}
							dst.gain += DELTA;
							dst.hold = true;
						};
						inline void SetPrevious(States::Knob &dst) const {
							if (dst.gain == MIN) {
								return;
							}
							dst.gain -= DELTA;
							dst.hold = true;
						};
						inline void Set(States::Knob &dst, States::Knob &src) const {
							dst = ::std::move(src);
							if (MAX < dst.gain) {
								dst.gain = MAX;
							} else if (dst.gain < MIN) {
								dst.gain = MIN;
							}
						};
					private:
						static constexpr int DEFAULT{ 0 };
						static constexpr int MIN{ -225 * SCALING_FACTOR };
						static constexpr int MAX{ 45 * SCALING_FACTOR };
						static constexpr int DELTA{ 1 };
					};
				}
			}
		}
	}
}

#endif
