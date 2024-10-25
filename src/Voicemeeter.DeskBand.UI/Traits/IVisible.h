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

					virtual const ::linear_algebra::vectord& get_Position() const = 0;
					virtual const ::linear_algebra::vectord& get_Size() const = 0;

					virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) = 0;

				protected:
					IVisible() = default;

					~IVisible() = default;
				};
			}
		}
	}
}