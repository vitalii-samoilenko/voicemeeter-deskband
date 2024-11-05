#pragma once

#include "estd/linear_algebra.h"

#include "Environment/IInputTracker.h"

#include "IComponent.h"
#include "IInputTracker.h"

namespace Voicemeeter {
	namespace UI {
		class InputTracker final : public IInputTracker {
		public:
			explicit InputTracker(
				::Environment::IInputTracker& envInputTracker
			);
			InputTracker(const InputTracker&) = delete;
			InputTracker(InputTracker&&) = delete;

			~InputTracker() = default;

			InputTracker& operator=(const InputTracker&) = delete;
			InputTracker& operator=(InputTracker&&) = delete;

			virtual void set_Focus(bool value) override;
			virtual const ::linear_algebra::vectord& get_Position() const override;
			virtual void set_Position(const ::linear_algebra::vectord& value) override;
			virtual bool get_Track(IComponent& component) const override;
			virtual void set_Track(IComponent& component, bool value) override;

			virtual bool MouseLDown(const ::linear_algebra::vectord& point) override;
			virtual bool MouseLDouble(const ::linear_algebra::vectord& point) override;
			virtual bool MouseMDown(const ::linear_algebra::vectord& point) override;
			virtual bool MouseMDouble(const ::linear_algebra::vectord& point) override;
			virtual bool MouseRDown(const ::linear_algebra::vectord& point) override;
			virtual bool MouseRDouble(const ::linear_algebra::vectord& point) override;
			virtual bool MouseWheel(const ::linear_algebra::vectord& point, int delta) override;
			virtual bool MouseMove(const ::linear_algebra::vectord& point) override;
			virtual bool MouseLUp(const ::linear_algebra::vectord& point) override;

		private:
			::Environment::IInputTracker& m_envInputTracker;
			IComponent* m_pTracked;
			::linear_algebra::vectord m_point;
		};
	}
}