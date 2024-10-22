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

				virtual ~Control() = 0;

				Control& operator=(const Control&) = delete;
				Control& operator=(Control&&) = delete;
			};
		}
	}
}