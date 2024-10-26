#pragma once

#include "estd/linear_algebra.h"

#include "../Controls/Carousel.h"

#include "IInteractivity.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				class CarouselInteractivity final : public IInteractivity<Controls::Carousel> {
				public:
					CarouselInteractivity() = default;
					CarouselInteractivity(const CarouselInteractivity&) = delete;
					CarouselInteractivity(CarouselInteractivity&&) = delete;

					~CarouselInteractivity() = default;

					CarouselInteractivity& operator=(const CarouselInteractivity&) = delete;
					CarouselInteractivity& operator=(CarouselInteractivity&&) = delete;

					virtual void MouseLDown(Controls::Carousel& component, const ::linear_algebra::vectord& point) const override;
					virtual void MouseLDouble(Controls::Carousel& component, const ::linear_algebra::vectord& point) const override;
					virtual void MouseRDown(Controls::Carousel& component, const ::linear_algebra::vectord& point) const override;
					virtual void MouseWheel(Controls::Carousel& component, const ::linear_algebra::vectord& point, int delta) const override;
					virtual void MouseMove(Controls::Carousel& component, const ::linear_algebra::vectord& point) const override;
					virtual void MouseLUp(Controls::Carousel& component, const ::linear_algebra::vectord& point) const override;
				};
			}
		}
	}
}