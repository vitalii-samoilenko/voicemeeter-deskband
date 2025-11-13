#ifndef VOICEMEETER_UI_POLICIES_STATE_CHANGES_STRIPKNOB_HPP
#define VOICEMEETER_UI_POLICIES_STATE_CHANGES_STRIPKNOB_HPP

#include <utility>

#include "Voicemeeter/UI/States/StripKnob.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace State {
				namespace Changes {
					struct StripKnob {
						inline void SetDefault(States::StripKnob &dst) const {
							if (dst.degree == DEFAULT) {
								return;
							}
							dst.degree = DEFAULT;
							dst.hold = true;
						};
						inline void SetNext(States::StripKnob &dst) const {
							if (dst.degree == MAX) {
								return;
							}
							dst.degree += DELTA;
							dst.hold = true;
						};
						inline void SetPrevious(States::StripKnob &dst) const {
							if (dst.degree == MIN) {
								return;
							}
							dst.degree -= DELTA;
							dst.hold = true;
						};
						inline void Set(States::StripKnob &dst, States::StripKnob &src) const {
							dst = ::std::move(src);
							if (MAX < dst.degree) {
								dst.gain = MAX;
							} else if (dst.degree < MIN) {
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
