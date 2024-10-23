#include "Component.h"

using namespace ::Voicemeeter::DeskBand::UI;

linear_algebra::vector Component::get_Position() const {
	return m_point;
}
linear_algebra::vector Component::get_Size() const {
	return m_vertex;
}
linear_algebra::vector Component::get_BaseSize() const {
	return m_baseVertex;
}

void Component::Redraw(linear_algebra::vector point, linear_algebra::vector vertex) {
	if (!linear_algebra::is_overlapping(point, vertex, m_point, m_vertex)) {
		return;
	}

	OnRedraw(point, vertex);
}
void Component::Rescale(linear_algebra::vector vertex) {
	OnRescale(vertex);
}
void Component::Move(linear_algebra::vector point) {
	m_point = point;

	OnMove(point);
}
bool Component::MouseLDown(linear_algebra::vector point) {
	if (!linear_algebra::is_inside(point - m_point, m_vertex)) {
		return false;
	}

	OnMouseLDown(point);

	return true;
}
bool Component::MouseRDown(linear_algebra::vector point) {
	if (!linear_algebra::is_inside(point - m_point, m_vertex)) {
		return false;
	}

	OnMouseRDown(point);

	return true;
}
bool Component::MouseWheel(linear_algebra::vector point, int delta) {
	if (!linear_algebra::is_inside(point - m_point, m_vertex)) {
		return false;
	}

	OnMouseWheel(point, delta);

	return true;
}