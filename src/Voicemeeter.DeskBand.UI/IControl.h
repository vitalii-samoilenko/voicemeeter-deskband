#pragma once

#include "IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class IControl : public IComponent {
			public:
				IControl(const IControl&) = delete;
				IControl(IControl&&) = delete;

				IControl& operator=(const IControl&) = delete;
				IControl& operator=(IControl&&) = delete;

			protected:
				IControl() = default;

				~IControl() = default;
			};
		}
	}
}