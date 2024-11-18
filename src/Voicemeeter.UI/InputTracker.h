#pragma once

#include "Environment/IInputTracker.h"

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
			virtual const ::std::valarray<double>& get_Position() const override;
			virtual void set_Position(const ::std::valarray<double>& value) override;
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
			::Environment::IInputTracker& m_envInputTracker;
			IComponent* m_pTracked;
			::std::valarray<double> m_point;
		};
	}
}