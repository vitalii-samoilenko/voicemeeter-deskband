#include <utility>

#include "Margin.h"

using namespace ::Voicemeeter::DeskBand::UI;

Margin::Margin(
	::std::unique_ptr<IComponent> pComponent,
	const ::linear_algebra::vectord& baseMarginTopLeft,
	const ::linear_algebra::vectord& baseMarginBottomRight
) : m_pComponent{ ::std::move(pComponent) }
  , m_position{ m_pComponent->get_Position() }
  , m_vertex{ m_pComponent->get_BaseSize() + baseMarginTopLeft + baseMarginBottomRight }
  , m_baseVertex{ m_vertex }
  , m_marginTopLeft{ baseMarginTopLeft }
  , m_marginBottomRight{ baseMarginBottomRight }
  , m_baseMarginTopLeft{ baseMarginTopLeft }
  , m_baseMarginBottomRight{ baseMarginBottomRight } {
	m_pComponent->Move(m_position + baseMarginTopLeft);
}

const ::linear_algebra::vectord& Margin::get_Position() const {
	return m_position;
}
const ::linear_algebra::vectord& Margin::get_Size() const {
	return m_vertex;
}
const ::linear_algebra::vectord& Margin::get_BaseSize() const {
	return m_baseVertex;
}

void Margin::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	m_pComponent->Redraw(point, vertex);
}
void Margin::Rescale(const ::linear_algebra::vectord& vertex) {
	double scale{
		::std::min<double>(
			vertex.x / m_baseVertex.x,
			vertex.y / m_baseVertex.y)
	};

	m_marginTopLeft = m_baseMarginTopLeft * scale;
	m_marginBottomRight = m_baseMarginBottomRight * scale;

	m_pComponent->Move(m_position + m_marginTopLeft);
	m_pComponent->Rescale(vertex - m_marginTopLeft - m_marginBottomRight);

	m_vertex = m_pComponent->get_Size() + m_marginTopLeft + m_marginBottomRight;
}
void Margin::Move(const ::linear_algebra::vectord& point) {
	m_position = point;

	m_pComponent->Move(point + m_marginTopLeft);
}
bool Margin::MouseLDown(const ::linear_algebra::vectord& point) {
	return m_pComponent->MouseLDown(point);
}
bool Margin::MouseLDouble(const ::linear_algebra::vectord& point) {
	return m_pComponent->MouseLDouble(point);
}
bool Margin::MouseRDown(const ::linear_algebra::vectord& point) {
	return m_pComponent->MouseRDown(point);
}
bool Margin::MouseWheel(const ::linear_algebra::vectord& point, int delta) {
	return m_pComponent->MouseWheel(point, delta);
}
bool Margin::MouseMove(const ::linear_algebra::vectord& point) {
	return m_pComponent->MouseMove(point);
}
bool Margin::MouseLUp(const ::linear_algebra::vectord& point) {
	return m_pComponent->MouseLUp(point);
}