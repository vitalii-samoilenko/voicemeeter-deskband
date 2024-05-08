#pragma once

#include "control.h"
#include "Presentation/statescene.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class StateControl : public Control {
				static std::once_flag CtlStateClassGuard;

				std::unique_ptr<Presentation::StateScene> m_pScene;
				LONG m_state;

			protected:
				inline virtual Presentation::StateScene& get_rScene() noexcept override {
					return *m_pScene.get();
				};
				inline void set_state(LONG state) noexcept {
					m_state = state;
				}

				explicit StateControl(
					Panel& rPnlParent
				);

			public:
				inline LONG get_state() const noexcept {
					return m_state;
				}

				StateControl() = delete;
				StateControl(const StateControl&) = delete;
				StateControl(StateControl&&) = delete;

				~StateControl() = default;

				StateControl& operator=(const StateControl&) = delete;
				StateControl& operator=(StateControl&&) = delete;
			};
		}
	}
}