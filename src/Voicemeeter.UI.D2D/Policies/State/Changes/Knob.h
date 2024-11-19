#pragma once

#include "Voicemeeter.UI/Policies/State/Changes/Knob.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				namespace State {
					namespace Changes {
						class Knob : public UI::Policies::State::Changes::Knob<9000, -13500, 13500, 1> {
						public:
							Knob() = default;
							Knob(const Knob&) = delete;
							Knob(Knob&&) = delete;

							~Knob() = default;

							Knob& operator=(const Knob&) = delete;
							Knob& operator=(Knob&&) = delete;

							virtual bool Set(States::Knob& dst, States::Knob& src) const override;
						};
					}
				}
			}
		}
	}
}