#include "InputTracker.h"

using namespace Voicemeeter::UI;

InputTracker::InputTracker(
	::Environment::IInputTracker& envInputTracker
) : m_envInputTracker{ envInputTracker }
  , m_pTracked{ nullptr }
  , m_point{ 0., 0. } {

}

void InputTracker::set_Focus(bool value) {
	if (!value) {
		m_envInputTracker.DisableInputTrack();
		m_pTracked = nullptr;
	}
}
const ::std::valarray<double>& InputTracker::get_Position() const {
	return m_point;
}
void InputTracker::set_Position(const ::std::valarray<double>& value) {
	m_point = value;
}
bool InputTracker::get_Track(IComponent& component) const {
	return m_pTracked == &component;
}
void InputTracker::set_Track(IComponent& component, bool value) {
	if (value) {
		if (!m_pTracked) {
			m_envInputTracker.EnableInputTrack();
		}
		m_pTracked = &component;
	} else if (m_pTracked == &component) {
		m_envInputTracker.DisableInputTrack();
		m_pTracked = nullptr;
	}
}
bool InputTracker::MouseLDown(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseLDown(point);
}
bool InputTracker::MouseLDouble(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseLDouble(point);
}
bool InputTracker::MouseMDown(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseMDown(point);
}
bool InputTracker::MouseMDouble(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseMDouble(point);
}
bool InputTracker::MouseRDown(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseRDown(point);
}
bool InputTracker::MouseRDouble(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseRDouble(point);
}
bool InputTracker::MouseWheel(const ::std::valarray<double>& point, int delta) {
	return m_pTracked
		&& m_pTracked->MouseWheel(point, delta);
}
bool InputTracker::MouseMove(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseMove(point);
}
bool InputTracker::MouseLUp(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseLUp(point);
}