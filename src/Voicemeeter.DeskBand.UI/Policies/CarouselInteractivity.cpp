#include "CarouselInteractivity.h"

using namespace ::Voicemeeter::DeskBand::UI::Policies;

void CarouselInteractivity::MouseLDown(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {
	component.SetNext();
}
void CarouselInteractivity::MouseLDouble(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {
	component.SetNext();
}
void CarouselInteractivity::MouseRDown(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {

}
void CarouselInteractivity::MouseWheel(Controls::Carousel& component, const ::linear_algebra::vectord& point, int delta) const {

}
void CarouselInteractivity::MouseMove(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {

}
void CarouselInteractivity::MouseLUp(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {

}