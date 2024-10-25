#include <utility>

#include "InputTracker.h"

using namespace Voicemeeter::DeskBand::UI;

InputTracker::InputTracker(
	IAppInputTracker& appInputTracler
) : m_appInputTracler{ appInputTracler } {

}

bool InputTracker::IsTracking(const IComponent& component) const {
	return &component == m_pPinned;
}

void InputTracker::EnableInputTrack(IComponent& component) {
	m_pPinned = &component;

	m_appInputTracler.EnableInputTrack();
}
void InputTracker::DisableInputTrack(IComponent& component) {
	m_pPinned = nullptr;

	m_appInputTracler.DisableInputTrack();
}
bool InputTracker::MouseLDown(const ::linear_algebra::vector& point) {
	return m_pPinned
		&& m_pPinned->MouseLDown(point);
}
bool InputTracker::MouseLDouble(const ::linear_algebra::vector& point) {
	return m_pPinned
		&& m_pPinned->MouseLDouble(point);
}
bool InputTracker::MouseRDown(const ::linear_algebra::vector& point) {
	return m_pPinned
		&& m_pPinned->MouseRDown(point);
}
bool InputTracker::MouseWheel(const ::linear_algebra::vector& point, int delta) {
	return m_pPinned
		&& m_pPinned->MouseWheel(point, delta);
}
bool InputTracker::MouseMove(const ::linear_algebra::vector& point) {
	return m_pPinned
		&& m_pPinned->MouseMove(point);
}
bool InputTracker::MouseLUp(const ::linear_algebra::vector& point) {
	return m_pPinned
		&& m_pPinned->MouseLUp(point);
}