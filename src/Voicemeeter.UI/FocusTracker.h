#pragma once

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

			virtual bool MouseLDown(const ::std::valarray<double>& point) override;
			virtual bool MouseLDouble(const ::std::valarray<double>& point) override;
			virtual bool MouseMDown(const ::std::valarray<double>& point) override;
			virtual bool MouseMDouble(const ::std::valarray<double>& point) override;
			virtual bool MouseRDown(const ::std::valarray<double>& point) override;
			virtual bool MouseRDouble(const ::std::valarray<double>& point) override;
			virtual bool MouseWheel(const ::std::valarray<double>& point, int delta) override;
			virtual bool MouseMove(const ::std::valarray<double>& point) override;
			virtual bool MouseLUp(const ::std::valarray<double>& point) override;

		private:
			IComponent* m_pTracked;
		};
	}
}