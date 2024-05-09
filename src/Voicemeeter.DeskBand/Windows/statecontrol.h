#pragma once

#include "control.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class StateControl : public Control {
				static std::once_flag CtlStateClassGuard;

				LONG m_state;

			protected:
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