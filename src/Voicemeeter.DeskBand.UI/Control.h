#pragma once

#include "Component.h"

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
					::linear_algebra::vector baseMarginTopLeft,
					::linear_algebra::vector baseMarginBottomRight
				) : Component{ baseMarginTopLeft, baseMarginBottomRight } {

				};

				~Control() = default;
			};
		}
	}
}