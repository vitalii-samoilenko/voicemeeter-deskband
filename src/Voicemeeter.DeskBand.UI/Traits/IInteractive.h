#pragma once

#include "estd/linear_algebra.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Traits {
				class IInteractive {
				public:
					IInteractive(const IInteractive&) = delete;
					IInteractive(IInteractive&&) = delete;

					IInteractive& operator=(const IInteractive&) = delete;
					IInteractive& operator=(IInteractive&&) = delete;

					virtual bool MouseLDown(const ::linear_algebra::vector& point) = 0;
					virtual bool MouseRDown(const ::linear_algebra::vector& point) = 0;
					virtual bool MouseWheel(const ::linear_algebra::vector& point, int delta) = 0;
					virtual void MouseMove(const ::linear_algebra::vector& point) = 0;
					virtual void MouseLUp(const ::linear_algebra::vector& point) = 0;

				protected:
					IInteractive() = default;

					~IInteractive() = default;
				};
			}
		}
	}
}