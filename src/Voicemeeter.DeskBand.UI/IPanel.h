#pragma once

#include "IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class IPanel : public IComponent {
			public:
				IPanel(const IPanel&) = delete;
				IPanel(IPanel&&) = delete;

				IPanel& operator=(const IPanel&) = delete;
				IPanel& operator=(IPanel&&) = delete;

			protected:
				IPanel() = default;

				~IPanel() = default;
			};
		}
	}
}