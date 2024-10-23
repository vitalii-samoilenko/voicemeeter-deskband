#include <utility>

#include "Component.h"

using namespace ::Voicemeeter::DeskBand::UI;

Component::Component(
	::linear_algebra::vector baseMarginTopLeft,
	::linear_algebra::vector baseMarginBottomRight
) : m_point{}
  , m_vertex{}
  , m_marginTopLeft{ baseMarginTopLeft }
  , m_marginBottomRight{ baseMarginBottomRight }
  , m_baseMarginTopLeft{ baseMarginTopLeft }
  , m_baseMarginBottomRight{ baseMarginBottomRight } {

}

::linear_algebra::vector Component::get_Position() const {
	return m_point;
}
::linear_algebra::vector Component::get_Size() const {
	return m_vertex;
}
::linear_algebra::vector Component::get_BaseSize() const {
	return OnGet_BaseSize() + m_baseMarginTopLeft + m_baseMarginBottomRight;
}

void Component::Redraw(::linear_algebra::vector point, ::linear_algebra::vector vertex) {
	if (!::linear_algebra::is_overlapping(
			point, vertex,
			m_point + m_marginTopLeft, m_vertex - m_marginBottomRight)) {
		return;
	}

	OnRedraw(point, vertex);
}
void Component::Rescale(::linear_algebra::vector vertex) {
	::linear_algebra::vector baseVertex{ get_BaseSize() };

	double scale{
		::std::min<double>(
			static_cast<double>(vertex.x) / baseVertex.x,
			static_cast<double>(vertex.y) / baseVertex.y)
	};

	vertex = {
		static_cast<int>(baseVertex.x * scale),
		static_cast<int>(baseVertex.y * scale)
	};
	m_marginTopLeft = {
		static_cast<int>(m_baseMarginTopLeft.x * scale),
		static_cast<int>(m_baseMarginTopLeft.y * scale)
	};
	m_marginBottomRight = {
		static_cast<int>(m_baseMarginTopLeft.x * scale),
		static_cast<int>(m_baseMarginTopLeft.y * scale)
	};
	m_vertex = OnRescale(vertex - m_marginTopLeft - m_marginBottomRight)
		+ m_marginTopLeft
		+ m_marginBottomRight;
}
void Component::Move(::linear_algebra::vector point) {
	m_point = point;

	OnMove(point + m_marginTopLeft);
}
bool Component::MouseLDown(::linear_algebra::vector point) {
	if (!::linear_algebra::is_inside(
			point - m_point,
			m_vertex)) {
		return false;
	}

	if (::linear_algebra::is_inside(
			point - m_point - m_marginTopLeft,
			m_vertex - m_marginBottomRight)) {
		OnMouseLDown(point);
	}

	return true;
}
bool Component::MouseRDown(::linear_algebra::vector point) {
	if (!::linear_algebra::is_inside(
			point - m_point,
			m_vertex)) {
		return false;
	}

	if (::linear_algebra::is_inside(
			point - m_point - m_marginTopLeft,
			m_vertex - m_marginBottomRight)) {
		OnMouseRDown(point);
	}

	return true;
}
bool Component::MouseWheel(::linear_algebra::vector point, int delta) {
	if (!::linear_algebra::is_inside(
			point - m_point,
			m_vertex)) {
		return false;
	}

	if (::linear_algebra::is_inside(
			point - m_point - m_marginTopLeft,
			m_vertex - m_marginBottomRight)) {
		OnMouseWheel(point, delta);
	}

	return true;
}