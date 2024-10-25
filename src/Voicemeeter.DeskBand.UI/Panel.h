#pragma once

#include "Component.h"
#include "IInputTracker.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class Panel : public Component {
			public:
				Panel(const Panel&) = delete;
				Panel(Panel&&) = delete;

				Panel& operator=(const Panel&) = delete;
				Panel& operator=(Panel&&) = delete;

			protected:
				Panel(
					IInputTracker& inputTracker,
					const ::linear_algebra::vectord& baseMarginTopLeft,
					const ::linear_algebra::vectord& baseMarginBottomRight
				) : Component{ inputTracker, baseMarginTopLeft, baseMarginBottomRight } {

				};

				~Panel() = default;
			};
		}
	}
}