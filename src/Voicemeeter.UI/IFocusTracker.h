#pragma once

#include "IComponent.h"
#include "Traits/IInteractive.h"

namespace Voicemeeter {
	namespace UI {
		class IFocusTracker : public Traits::IInteractive {
		public:
			IFocusTracker(const IFocusTracker&) = delete;
			IFocusTracker(IFocusTracker&&) = delete;

			virtual ~IFocusTracker() = default;

			IFocusTracker& operator=(const IFocusTracker&) = delete;
			IFocusTracker& operator=(IFocusTracker&&) = delete;

			virtual bool get_Track(IComponent& component) const = 0;
			virtual void set_Track(IComponent& component, bool value) = 0;

		protected:
			IFocusTracker() = default;
		};
	}
}