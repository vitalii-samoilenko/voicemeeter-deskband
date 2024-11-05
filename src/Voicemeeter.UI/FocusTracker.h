#pragma once

#include "estd/linear_algebra.h"

#include "IComponent.h"
#include "IFocusTracker.h"

namespace Voicemeeter {
	namespace UI {
		class FocusTracker final : public IFocusTracker {
		public:
			FocusTracker();
			FocusTracker(const FocusTracker&) = delete;
			FocusTracker(FocusTracker&&) = delete;

			~FocusTracker() = default;

			FocusTracker& operator=(const FocusTracker&) = delete;
			FocusTracker& operator=(FocusTracker&&) = delete;

			virtual void set_Focus(bool value) override;
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
			IComponent* m_pTracked;
		};
	}
}