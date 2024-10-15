#pragma once

#include "Traits/IVisible.h"
#include "Traits/IScalable.h"
#include "Traits/IMovable.h"
#include "Traits/IInteractive.h"

using namespace Voicemeeter::DeskBand::UI::Traits;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class IComponent : public IVisible, public IScalable, public IMovable, public IInteractive {
			public:
				IComponent() = delete;
				IComponent(const IComponent&) = delete;
				IComponent(IComponent&&) = delete;

				virtual ~IComponent() = 0;

				IComponent& operator=(const IComponent&) = delete;
				IComponent& operator=(IComponent&&) = delete;
			};
		}
	}
}