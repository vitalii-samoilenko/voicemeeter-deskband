#include <utility>

#include "InputTracker.h"

using namespace Voicemeeter::UI;

InputTracker::InputTracker(
	::Environment::IInputTracker& envInputTracler
) : m_envInputTracler{ envInputTracler }
  , m_pPinned{ nullptr }
  , m_point{ ::linear_algebra::vectord::origin() } {

}

const ::linear_algebra::vectord& InputTracker::get_PinPosition() const {
	return m_point;
};

bool InputTracker::IsTracking(const IComponent& component) const {
	return &component == m_pPinned;
}

void InputTracker::EnableInputTrack(IComponent& component, const ::linear_algebra::vectord& point) {
	if (!m_pPinned) {
		m_envInputTracler.EnableInputTrack();
	}

	m_point = point;
	m_pPinned = &component;
}
void InputTracker::DisableInputTrack(IComponent& component) {
	m_envInputTracler.DisableInputTrack();

	m_pPinned = nullptr;
}
bool InputTracker::MouseLDown(const ::linear_algebra::vectord& point) {
	return m_pPinned
		&& m_pPinned->MouseLDown(point);
}
bool InputTracker::MouseLDouble(const ::linear_algebra::vectord& point) {
	return m_pPinned
		&& m_pPinned->MouseLDouble(point);
}
bool InputTracker::MouseRDown(const ::linear_algebra::vectord& point) {
	return m_pPinned
		&& m_pPinned->MouseRDown(point);
}
bool InputTracker::MouseRDouble(const ::linear_algebra::vectord& point) {
	return m_pPinned
		&& m_pPinned->MouseRDouble(point);
}
bool InputTracker::MouseWheel(const ::linear_algebra::vectord& point, int delta) {
	return m_pPinned
		&& m_pPinned->MouseWheel(point, delta);
}
bool InputTracker::MouseMove(const ::linear_algebra::vectord& point) {
	return m_pPinned
		&& m_pPinned->MouseMove(point);
}
bool InputTracker::MouseLUp(const ::linear_algebra::vectord& point) {
	return m_pPinned
		&& m_pPinned->MouseLUp(point);
}