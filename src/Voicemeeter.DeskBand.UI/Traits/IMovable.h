#pragma once

#include "estd/linear_algebra.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Traits {
				class IMovable {
				public:
					IMovable(const IMovable&) = delete;
					IMovable(IMovable&&) = delete;

					IMovable& operator=(const IMovable&) = delete;
					IMovable& operator=(IMovable&&) = delete;

					virtual void Move(const ::linear_algebra::vectord& point) = 0;

				protected:
					IMovable() = default;

					~IMovable() = default;
				};
			}
		}
	}
}