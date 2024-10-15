#pragma once

#include "linear_algebra.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Traits {
				class IScalable {
				public:
					IScalable() = delete;
					IScalable(const IScalable&) = delete;
					IScalable(IScalable&&) = delete;

					~IScalable() = delete;

					IScalable& operator=(const IScalable&) = delete;
					IScalable& operator=(IScalable&&) = delete;

					const virtual linear_algebra::vector get_BaseSize() = 0;

					virtual void Rescale(linear_algebra::vector size) = 0;
				};
			}
		}
	}
}