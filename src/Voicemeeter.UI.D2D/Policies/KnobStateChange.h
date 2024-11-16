#pragma once

#include "Voicemeeter.UI/Policies/KnobStateChange.h"

using namespace Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				class KnobStateChange : public UI::Policies::KnobStateChange<9000, -13500, 13500, 1> {
				public:
					KnobStateChange() = default;
					KnobStateChange(const KnobStateChange&) = delete;
					KnobStateChange(KnobStateChange&&) = delete;

					~KnobStateChange() = default;

					KnobStateChange& operator=(const KnobStateChange&) = delete;
					KnobStateChange& operator=(KnobStateChange&&) = delete;

					virtual bool Set(States::Knob& dst, States::Knob& src) const override;
				};
			}
		}
	}
}