#pragma once

#include "Environment/IInputTracker.h"

#include "IInput.h"

namespace Voicemeeter {
	namespace UI {
		namespace Trackers {
			class Input final : public IInput {
			public:
				explicit Input(
					::Environment::IInputTracker& envInputTracker
				);
				Input(const Input&) = delete;
				Input(Input&&) = delete;

				~Input() = default;

				Input& operator=(const Input&) = delete;
				Input& operator=(Input&&) = delete;

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
}