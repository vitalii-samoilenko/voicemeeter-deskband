#include <utility>

#include "RegionCheck.h"

using namespace ::Voicemeeter::DeskBand::UI;

static inline bool is_inside(const ::linear_algebra::vectord& point, const IComponent& component) {
	return ::linear_algebra::is_inside(
		point - component.get_Position(),
		component.get_Size());
}

RegionCheck::RegionCheck(
	::std::unique_ptr<IComponent> pComponent
) : m_pComponent{ ::std::move(pComponent) } {

}

const ::linear_algebra::vectord& RegionCheck::get_Position() const {
	return m_pComponent->get_Position();
}
const ::linear_algebra::vectord& RegionCheck::get_Size() const {
	return m_pComponent->get_Size();
}
const ::linear_algebra::vectord& RegionCheck::get_BaseSize() const {
	return m_pComponent->get_BaseSize();
}

void RegionCheck::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	if (!::linear_algebra::is_overlapping(
			point, vertex,
			m_pComponent->get_Position(), m_pComponent->get_Size())) {
		return;
	}

	m_pComponent->Redraw(point, vertex);
}
void RegionCheck::Rescale(const ::linear_algebra::vectord& vertex) {
	m_pComponent->Rescale(vertex);
}
void RegionCheck::Move(const ::linear_algebra::vectord& point) {
	m_pComponent->Move(point);
}
bool RegionCheck::MouseLDown(const ::linear_algebra::vectord& point) {
	return is_inside(point, *m_pComponent)
		&& m_pComponent->MouseLDown(point);
}
bool RegionCheck::MouseLDouble(const ::linear_algebra::vectord& point) {
	return is_inside(point, *m_pComponent)
		&& m_pComponent->MouseLDouble(point);
}
bool RegionCheck::MouseRDown(const ::linear_algebra::vectord& point) {
	return is_inside(point, *m_pComponent)
		&& m_pComponent->MouseRDown(point);
}
bool RegionCheck::MouseWheel(const ::linear_algebra::vectord& point, int delta) {
	return is_inside(point, *m_pComponent)
		&& m_pComponent->MouseWheel(point, delta);
}
bool RegionCheck::MouseMove(const ::linear_algebra::vectord& point) {
	return is_inside(point, *m_pComponent)
		&& m_pComponent->MouseMove(point);
}
bool RegionCheck::MouseLUp(const ::linear_algebra::vectord& point) {
	return is_inside(point, *m_pComponent)
		&& m_pComponent->MouseLUp(point);
}