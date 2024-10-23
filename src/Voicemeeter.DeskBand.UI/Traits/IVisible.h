#pragma once

#include "estd/linear_algebra.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Traits {
				class IVisible {
				public:
					IVisible(const IVisible&) = delete;
					IVisible(IVisible&&) = delete;

					IVisible& operator=(const IVisible&) = delete;
					IVisible& operator=(IVisible&&) = delete;

					virtual ::linear_algebra::vector get_Position() const = 0;
					virtual ::linear_algebra::vector get_Size() const = 0;

					virtual void Redraw(::linear_algebra::vector point, ::linear_algebra::vector vertex) = 0;

				protected:
					IVisible() = default;

					~IVisible() = default;
				};
			}
		}
	}
}