#pragma once

#include "linear_algebra.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Traits {
				class IInteractive {
				public:
					IInteractive() = delete;
					IInteractive(const IInteractive&) = delete;
					IInteractive(IInteractive&&) = delete;

					~IInteractive() = delete;

					IInteractive& operator=(const IInteractive&) = delete;
					IInteractive& operator=(IInteractive&&) = delete;

					virtual bool MouseLDown(linear_algebra::vector point) = 0;
					virtual bool MouseRDown(linear_algebra::vector point) = 0;
					virtual bool MouseWheel(linear_algebra::vector point, int delta) = 0;
					virtual bool MouseMove(linear_algebra::vector point) = 0;
					virtual bool MouseLUp(linear_algebra::vector point) = 0;
				};
			}
		}
	}
}