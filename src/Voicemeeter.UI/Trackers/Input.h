#pragma once

#include "Environment/IInputTracker.h"

#include "../IComponent.h"
#include "../Traits/IInteractive.h"

namespace Voicemeeter {
	namespace UI {
		namespace Trackers {
			class Input final : public Traits::IInteractive {
			public:
				explicit Input(
					::Environment::IInputTracker& envInputTracker
				);
				Input(const Input&) = delete;
				Input(Input&&) = delete;

				~Input() = default;

				Input& operator=(const Input&) = delete;
				Input& operator=(Input&&) = delete;

				void set_Focus(bool value) override;
				inline const ::std::valarray<double>& get_Position() const {
					return m_point;
				};
				inline void set_Position(const ::std::valarray<double>& value) {
					m_point = value;
				};
				inline bool get_Track(IComponent& component) const {
					return m_pTracked == &component;
				};
				inline void set_Track(IComponent& component, bool value) {
					if (value) {
						if (!m_pTracked) {
							m_envInputTracker.EnableInputTrack();
						}
						m_pTracked = &component;
					} else if (m_pTracked == &component) {
						m_envInputTracker.DisableInputTrack();
						m_pTracked = nullptr;
					}
				};

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