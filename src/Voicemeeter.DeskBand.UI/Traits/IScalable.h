#pragma once

#include "estd/linear_algebra.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Traits {
				class IScalable {
				public:
					IScalable(const IScalable&) = delete;
					IScalable(IScalable&&) = delete;

					IScalable& operator=(const IScalable&) = delete;
					IScalable& operator=(IScalable&&) = delete;

					virtual const ::linear_algebra::vectord& get_BaseSize() const = 0;

					virtual void Rescale(const ::linear_algebra::vectord& vertex) = 0;

				protected:
					IScalable() = default;

					~IScalable() = default;
				};
			}
		}
	}
}