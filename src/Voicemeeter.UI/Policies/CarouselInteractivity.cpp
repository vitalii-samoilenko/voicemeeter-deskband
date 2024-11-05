#include "CarouselInteractivity.h"

using namespace ::Voicemeeter::UI::Policies;

CarouselInteractivity::CarouselInteractivity(
	IFocusTracker& focusTracker
) : m_focusTracker{ focusTracker } {

}

void CarouselInteractivity::set_Focus(Controls::Carousel& component, bool value) const {

};

void CarouselInteractivity::MouseLDown(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {
	m_focusTracker.set_Track(component, true);

	component.SetNext();
}
void CarouselInteractivity::MouseLDouble(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {
	component.SetNext();
}
void CarouselInteractivity::MouseMDown(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {

}
void CarouselInteractivity::MouseMDouble(Controls::Carousel& component, const ::linear_algebra::vectord& point) const {

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