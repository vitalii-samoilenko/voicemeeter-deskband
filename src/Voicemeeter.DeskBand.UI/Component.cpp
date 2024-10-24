#include <utility>

#include "Component.h"

using namespace ::Voicemeeter::DeskBand::UI;

Component::Component(
	const ::linear_algebra::vector& baseMarginTopLeft,
	const ::linear_algebra::vector& baseMarginBottomRight
) : m_marginTopLeft{ baseMarginTopLeft }
  , m_marginBottomRight{ baseMarginBottomRight }
  , m_baseMarginTopLeft{ baseMarginTopLeft }
  , m_baseMarginBottomRight{ baseMarginBottomRight } {

}

const ::linear_algebra::vector& Component::get_Position() const {
	return OnGet_Position() - m_marginTopLeft;
}
const ::linear_algebra::vector& Component::get_Size() const {
	return OnGet_Size() + m_marginTopLeft + m_marginBottomRight;
}
const ::linear_algebra::vector& Component::get_BaseSize() const {
	return OnGet_BaseSize() + m_baseMarginTopLeft + m_baseMarginBottomRight;
}

void Component::Redraw(const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) {
	if (!::linear_algebra::is_overlapping(
			point, vertex,
			OnGet_Position(), OnGet_Size())) {
		return;
	}

	OnRedraw(point, vertex);
}
void Component::Rescale(const ::linear_algebra::vector& vertex) {
	::linear_algebra::vector point{ get_Position() };
	::linear_algebra::vector baseVertex{ get_BaseSize() };

	double scale{
		::std::min<double>(
			static_cast<double>(vertex.x) / baseVertex.x,
			static_cast<double>(vertex.y) / baseVertex.y)
	};

	m_marginTopLeft.x = m_baseMarginTopLeft.x * scale;
	m_marginTopLeft.y = m_baseMarginTopLeft.y * scale;
	m_marginBottomRight.x = m_baseMarginTopLeft.x * scale;
	m_marginBottomRight.y = m_baseMarginTopLeft.y * scale;

	Move(point);
	OnRescale(vertex - m_marginTopLeft - m_marginBottomRight);
}
void Component::Move(const ::linear_algebra::vector& point) {
	OnMove(point + m_marginTopLeft);
}
bool Component::MouseLDown(const ::linear_algebra::vector& point) {
	if (!::linear_algebra::is_inside(
			point - get_Position(),
			get_Size())) {
		return false;
	}

	if (::linear_algebra::is_inside(
			point - OnGet_Position(),
			OnGet_Size())) {
		OnMouseLDown(point);
	}

	return true;
}
bool Component::MouseRDown(const ::linear_algebra::vector& point) {
	if (!::linear_algebra::is_inside(
			point - get_Position(),
			get_Size())) {
		return false;
	}

	if (::linear_algebra::is_inside(
			point - OnGet_Position(),
			OnGet_Size())) {
		OnMouseRDown(point);
	}

	return true;
}
bool Component::MouseWheel(const ::linear_algebra::vector& point, int delta) {
	if (!::linear_algebra::is_inside(
			point - get_Position(),
			get_Size())) {
		return false;
	}

	if (::linear_algebra::is_inside(
			point - OnGet_Position(),
			OnGet_Size())) {
		OnMouseWheel(point, delta);
	}

	return true;
}