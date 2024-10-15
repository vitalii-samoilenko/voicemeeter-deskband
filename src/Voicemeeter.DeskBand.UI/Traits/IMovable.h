#pragma once

#include "linear_algebra.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Traits {
				class IMovable {
				public:
					IMovable() = delete;
					IMovable(const IMovable&) = delete;
					IMovable(IMovable&&) = delete;

					~IMovable() = delete;

					IMovable& operator=(const IMovable&) = delete;
					IMovable& operator=(IMovable&&) = delete;

					virtual void Move(linear_algebra::vector position) = 0;
				};
			}
		}
	}
}