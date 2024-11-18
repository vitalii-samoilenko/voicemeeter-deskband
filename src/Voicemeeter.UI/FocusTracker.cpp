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
bool FocusTracker::MouseLDown(const ::std::valarray<double>& point) {
	return false;
}
bool FocusTracker::MouseLDouble(const ::std::valarray<double>& point) {
	return false;
}
bool FocusTracker::MouseMDown(const ::std::valarray<double>& point) {
	return false;
}
bool FocusTracker::MouseMDouble(const ::std::valarray<double>& point) {
	return false;
}
bool FocusTracker::MouseRDown(const ::std::valarray<double>& point) {
	return false;
}
bool FocusTracker::MouseRDouble(const ::std::valarray<double>& point) {
	return false;
}
bool FocusTracker::MouseWheel(const ::std::valarray<double>& point, int delta) {
	return false;
}
bool FocusTracker::MouseMove(const ::std::valarray<double>& point) {
	return false;
}
bool FocusTracker::MouseLUp(const ::std::valarray<double>& point) {
	return false;
}