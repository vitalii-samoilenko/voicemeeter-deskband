#include <algorithm>

#include <d2d1_3.h>

#include "Glyph.h"

#pragma comment(lib, "d2d1")

using namespace ::Voicemeeter::UI::D2D::Graphics;

Glyph::Glyph(
	Canvas& canvas,
	const ::linear_algebra::vectord& baseVertex
) : m_canvas{ canvas }
  , m_point{}
  , m_vertex{ baseVertex }
  , m_baseVertex{ baseVertex } {

}

const ::linear_algebra::vectord& Glyph::get_Position() const {
	return m_point;
}
const ::linear_algebra::vectord& Glyph::get_Size() const {
	return m_vertex;
}
const ::linear_algebra::vectord& Glyph::get_BaseSize() const {
	return m_baseVertex;
}

void Glyph::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	m_canvas.get_pRenderTarget()
		->SetTransform(
			::D2D1::Matrix3x2F::Scale(
				static_cast<FLOAT>(m_vertex.x / m_baseVertex.x),
				static_cast<FLOAT>(m_vertex.y / m_baseVertex.y))
			* ::D2D1::Matrix3x2F::Translation(
				static_cast<FLOAT>(m_point.x),
				static_cast<FLOAT>(m_point.y)));
}
void Glyph::Move(const ::linear_algebra::vectord& point) {
	m_point = point;
}
void Glyph::Rescale(const ::linear_algebra::vectord& vertex) {
	double scale{
		::std::min<double>(
			vertex.x / m_baseVertex.x,
			vertex.y / m_baseVertex.y)
	};

	m_vertex = m_baseVertex * scale;
}