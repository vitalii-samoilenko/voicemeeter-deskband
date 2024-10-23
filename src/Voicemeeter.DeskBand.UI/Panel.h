#pragma once

#include "Component.h"

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
					::linear_algebra::vector baseMarginTopLeft,
					::linear_algebra::vector baseMarginBottomRight
				) : Component{ baseMarginTopLeft, baseMarginBottomRight } {

				};

				~Panel() = default;
			};
		}
	}
}