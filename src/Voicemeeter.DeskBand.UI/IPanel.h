#pragma once

#include "IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class IPanel : public IComponent {
			public:
				IPanel() = delete;
				IPanel(const IPanel&) = delete;
				IPanel(IPanel&&) = delete;

				virtual ~IPanel() = 0;

				IPanel& operator=(const IPanel&) = delete;
				IPanel& operator=(IPanel&&) = delete;
			};
		}
	}
}