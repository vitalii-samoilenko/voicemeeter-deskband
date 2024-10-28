#pragma once

#include "estd/linear_algebra.h"

namespace Voicemeeter {
	namespace UI {
		namespace Traits {
			class IInteractive {
			public:
				IInteractive(const IInteractive&) = delete;
				IInteractive(IInteractive&&) = delete;

				IInteractive& operator=(const IInteractive&) = delete;
				IInteractive& operator=(IInteractive&&) = delete;

				virtual bool MouseLDown(const ::linear_algebra::vectord& point) = 0;
				virtual bool MouseLDouble(const ::linear_algebra::vectord& point) = 0;
				virtual bool MouseRDown(const ::linear_algebra::vectord& point) = 0;
				virtual bool MouseRDouble(const ::linear_algebra::vectord& point) = 0;
				virtual bool MouseWheel(const ::linear_algebra::vectord& point, int delta) = 0;
				virtual bool MouseMove(const ::linear_algebra::vectord& point) = 0;
				virtual bool MouseLUp(const ::linear_algebra::vectord& point) = 0;

			protected:
				IInteractive() = default;

				~IInteractive() = default;
			};
		}
	}
}