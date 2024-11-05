#include "FocusTracker.h"

using namespace Voicemeeter::UI;

FocusTracker::FocusTracker(
) : m_pTracked{ nullptr } {

}

void FocusTracker::set_Focus(bool value) {
	if (!value) {
		if (m_pTracked) {
			m_pTracked->set_Focus(false);
			m_pTracked = nullptr;
		}
	}
}
bool FocusTracker::get_Track(IComponent& component) const {
	return m_pTracked == &component;
}
void FocusTracker::set_Track(IComponent& component, bool value) {
	if (value) {
		if (m_pTracked == &component) {
			return;
		}
		if (m_pTracked) {
			m_pTracked->set_Focus(false);
		}
		m_pTracked = &component;
		m_pTracked->set_Focus(true);
	} else if (m_pTracked == &component) {
		m_pTracked->set_Focus(false);
		m_pTracked = nullptr;
	}
}
bool FocusTracker::MouseLDown(const ::linear_algebra::vectord& point) {
	return false;
}
bool FocusTracker::MouseLDouble(const ::linear_algebra::vectord& point) {
	return false;
}
bool FocusTracker::MouseMDown(const ::linear_algebra::vectord& point) {
	return false;
}
bool FocusTracker::MouseMDouble(const ::linear_algebra::vectord& point) {
	return false;
}
bool FocusTracker::MouseRDown(const ::linear_algebra::vectord& point) {
	return false;
}
bool FocusTracker::MouseRDouble(const ::linear_algebra::vectord& point) {
	return false;
}
bool FocusTracker::MouseWheel(const ::linear_algebra::vectord& point, int delta) {
	return false;
}
bool FocusTracker::MouseMove(const ::linear_algebra::vectord& point) {
	return false;
}
bool FocusTracker::MouseLUp(const ::linear_algebra::vectord& point) {
	return false;
}