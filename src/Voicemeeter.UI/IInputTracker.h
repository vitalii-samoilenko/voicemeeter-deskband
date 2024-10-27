#pragma once

#include "estd/linear_algebra.h"

#include "Traits/IInteractive.h"

#include "IComponent.h"

namespace Voicemeeter {
	namespace UI {
		class IInputTracker : public Traits::IInteractive {
		public:
			IInputTracker(const IInputTracker&) = delete;
			IInputTracker(IInputTracker&&) = delete;

			virtual ~IInputTracker() = default;

			IInputTracker& operator=(const IInputTracker&) = delete;
			IInputTracker& operator=(IInputTracker&&) = delete;

			virtual const ::linear_algebra::vectord& get_PinPosition() const = 0;

			virtual bool IsTracking(const IComponent& component) const = 0;

			virtual void EnableInputTrack(IComponent& component, const ::linear_algebra::vectord& point) = 0;
			virtual void DisableInputTrack(IComponent& component) = 0;

		protected:
			IInputTracker() = default;
		};
	}
}