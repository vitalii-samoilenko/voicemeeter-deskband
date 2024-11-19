#pragma once

#include "../IComponent.h"
#include "../Traits/IInteractive.h"

namespace Voicemeeter {
	namespace UI {
		namespace Trackers {
			class IFocus : public Traits::IInteractive {
			public:
				IFocus(const IFocus&) = delete;
				IFocus(IFocus&&) = delete;

				virtual ~IFocus() = default;

				IFocus& operator=(const IFocus&) = delete;
				IFocus& operator=(IFocus&&) = delete;

				virtual bool get_Track(IComponent& component) const = 0;
				virtual void set_Track(IComponent& component, bool value) = 0;

			protected:
				IFocus() = default;
			};
		}
	}
}