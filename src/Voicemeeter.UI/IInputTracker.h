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

			virtual const ::linear_algebra::vectord& get_Position() const = 0;
			virtual void set_Position(const ::linear_algebra::vectord& value) = 0;
			virtual bool get_Track(IComponent& component) const = 0;
			virtual void set_Track(IComponent& component, bool value) = 0;

		protected:
			IInputTracker() = default;
		};
	}
}