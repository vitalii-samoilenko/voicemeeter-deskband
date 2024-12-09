#include "Input.h"

using namespace ::Voicemeeter::UI::Trackers;

Input::Input(
	::Environment::IInputTracker& envInputTracker
) : m_envInputTracker{ envInputTracker }
  , m_pTracked{ nullptr }
  , m_point{ 0., 0. } {

}

void Input::set_Focus(bool value) {
	if (value) {
		return;
	}
	if (m_pTracked) {
		m_envInputTracker.DisableInputTrack();
		m_pTracked = nullptr;
	}
}

bool Input::MouseLDown(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseLDown(point);
}
bool Input::MouseLDouble(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseLDouble(point);
}
bool Input::MouseMDown(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseMDown(point);
}
bool Input::MouseMDouble(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseMDouble(point);
}
bool Input::MouseRDown(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseRDown(point);
}
bool Input::MouseRDouble(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseRDouble(point);
}
bool Input::MouseWheel(const ::std::valarray<double>& point, int delta) {
	return m_pTracked
		&& m_pTracked->MouseWheel(point, delta);
}
bool Input::MouseMove(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseMove(point);
}
bool Input::MouseLUp(const ::std::valarray<double>& point) {
	return m_pTracked
		&& m_pTracked->MouseLUp(point);
}