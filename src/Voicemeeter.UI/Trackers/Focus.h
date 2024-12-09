#pragma once

#include "../IComponent.h"
#include "../Traits/IInteractive.h"

namespace Voicemeeter {
	namespace UI {
		namespace Trackers {
			class Focus final : public Traits::IInteractive {
			public:
				Focus();
				Focus(const Focus&) = delete;
				Focus(Focus&&) = delete;

				~Focus() = default;

				Focus& operator=(const Focus&) = delete;
				Focus& operator=(Focus&&) = delete;

				virtual void set_Focus(bool value) override;
				inline bool get_Track(IComponent& component) const {
					return m_pTracked == &component;
				};
				inline void set_Track(IComponent& component, bool value) {
					if (value) {
						if (m_pTracked == &component) {
							return;
						}
						if (m_pTracked) {
							m_pTracked->set_Focus(false);
						}
						m_pTracked = &component;
						m_pTracked->set_Focus(true);
					} else if (m_pTracked == &component) {
						m_pTracked->set_Focus(false);
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
				IComponent* m_pTracked;
			};
		}
	}
}