#pragma once

#include "estd/linear_algebra.h"

#include "IComponent.h"
#include "IInputTracker.h"
#include "ISystemInputTracker.h"

namespace Voicemeeter {
	namespace UI {
		class InputTracker final : public IInputTracker {
		public:
			explicit InputTracker(
				ISystemInputTracker& systemInputTracler
			);
			InputTracker(const InputTracker&) = delete;
			InputTracker(InputTracker&&) = delete;

			~InputTracker() = default;

			InputTracker& operator=(const InputTracker&) = delete;
			InputTracker& operator=(InputTracker&&) = delete;

			virtual const ::linear_algebra::vectord& get_PinPosition() const override;

			virtual bool IsTracking(const IComponent& component) const override;

			virtual void EnableInputTrack(IComponent& component, const ::linear_algebra::vectord& point) override;
			virtual void DisableInputTrack(IComponent& component) override;
			virtual bool MouseLDown(const ::linear_algebra::vectord& point) override;
			virtual bool MouseLDouble(const ::linear_algebra::vectord& point) override;
			virtual bool MouseRDown(const ::linear_algebra::vectord& point) override;
			virtual bool MouseWheel(const ::linear_algebra::vectord& point, int delta) override;
			virtual bool MouseMove(const ::linear_algebra::vectord& point) override;
			virtual bool MouseLUp(const ::linear_algebra::vectord& point) override;

		private:
			ISystemInputTracker& m_systemInputTracler;
			IComponent* m_pPinned;
			::linear_algebra::vectord m_point;
		};
	}
}