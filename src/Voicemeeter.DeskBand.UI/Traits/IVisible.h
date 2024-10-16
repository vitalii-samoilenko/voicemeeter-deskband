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

					const virtual linear_algebra::vector get_Position() = 0;
					const virtual linear_algebra::vector get_Size() = 0;

					virtual void Redraw(linear_algebra::vector region) = 0;

				protected:
					IVisible() = default;

					~IVisible() = default;
				};
			}
		}
	}
}