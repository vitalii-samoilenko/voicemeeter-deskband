#pragma once

#include "Component.h"
#include "IInputTracker.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class Control : public Component {
			public:
				Control() = delete;
				Control(const Control&) = delete;
				Control(Control&&) = delete;

				Control& operator=(const Control&) = delete;
				Control& operator=(Control&&) = delete;

			protected:
				Control(
					IInputTracker& inputTracker,
					const ::linear_algebra::vectord& baseMarginTopLeft,
					const ::linear_algebra::vectord& baseMarginBottomRight
				) : Component{ inputTracker, baseMarginTopLeft, baseMarginBottomRight } {

				};

				~Control() = default;
			};
		}
	}
}