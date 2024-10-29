#include "CarouselInteractivity.h"

using namespace ::Voicemeeter::UI::Policies;

CarouselInteractivity::CarouselInteractivity(
	::Environment::ITimer& timer
) : m_timer{ timer } {

}

void CarouselInteractivity::MouseLDown(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {
	m_timer.Elapse();
	component.SetNext();
}
void CarouselInteractivity::MouseLDouble(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {
	m_timer.Elapse();
	component.SetNext();
}
void CarouselInteractivity::MouseRDown(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {

}
void CarouselInteractivity::MouseRDouble(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {
	
}
void CarouselInteractivity::MouseWheel(Controls::Carousel& component, const ::linear_algebra::vectord& point, int delta) const {

}
void CarouselInteractivity::MouseMove(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {

}
void CarouselInteractivity::MouseLUp(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {

}