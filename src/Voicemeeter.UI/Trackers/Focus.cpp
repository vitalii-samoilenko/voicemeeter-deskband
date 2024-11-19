#include "Focus.h"

using namespace ::Voicemeeter::UI::Trackers;

Focus::Focus(
) : m_pTracked{ nullptr } {

}

void Focus::set_Focus(bool value) {
	if (!value) {
		if (m_pTracked) {
			m_pTracked->set_Focus(false);
			m_pTracked = nullptr;
		}
	}
}
bool Focus::get_Track(IComponent& component) const {
	return m_pTracked == &component;
}
void Focus::set_Track(IComponent& component, bool value) {
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
bool Focus::MouseLDown(const ::std::valarray<double>& point) {
	return false;
}
bool Focus::MouseLDouble(const ::std::valarray<double>& point) {
	return false;
}
bool Focus::MouseMDown(const ::std::valarray<double>& point) {
	return false;
}
bool Focus::MouseMDouble(const ::std::valarray<double>& point) {
	return false;
}
bool Focus::MouseRDown(const ::std::valarray<double>& point) {
	return false;
}
bool Focus::MouseRDouble(const ::std::valarray<double>& point) {
	return false;
}
bool Focus::MouseWheel(const ::std::valarray<double>& point, int delta) {
	return false;
}
bool Focus::MouseMove(const ::std::valarray<double>& point) {
	return false;
}
bool Focus::MouseLUp(const ::std::valarray<double>& point) {
	return false;
}