#pragma once

#include "IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class IControl : public IComponent {
			public:
				IControl() = delete;
				IControl(const IControl&) = delete;
				IControl(IControl&&) = delete;

				virtual ~IControl() = 0;

				IControl& operator=(const IControl&) = delete;
				IControl& operator=(IControl&&) = delete;
			};
		}
	}
}