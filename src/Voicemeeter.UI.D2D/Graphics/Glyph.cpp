#include <d2d1_3.h>

#include "Glyph.h"

#pragma comment(lib, "d2d1")

using namespace ::Voicemeeter::UI::D2D::Graphics;

Glyph::Glyph(
	const Canvas& canvas,
	const ::std::valarray<double>& baseVertex
) : m_canvas{ canvas }
  , m_point{ 0., 0. }
  , m_vertex{ baseVertex }
  , m_baseVertex{ baseVertex } {

}

const ::std::valarray<double>& Glyph::get_Position() const {
	return m_point;
}
const ::std::valarray<double>& Glyph::get_Size() const {
	return m_vertex;
}
const ::std::valarray<double>& Glyph::get_BaseSize() const {
	return m_baseVertex;
}

void Glyph::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	m_canvas.get_pD2dDeviceContext()
		->SetTransform(
			::D2D1::Matrix3x2F::Scale(
				static_cast<FLOAT>(m_vertex[0] / m_baseVertex[0]),
				static_cast<FLOAT>(m_vertex[1] / m_baseVertex[1]))
			* ::D2D1::Matrix3x2F::Translation(
				static_cast<FLOAT>(m_point[0]),
				static_cast<FLOAT>(m_point[1])));
}
void Glyph::Move(const ::std::valarray<double>& point) {
	m_point = point;
}
void Glyph::Rescale(const ::std::valarray<double>& vertex) {
	double scale{ (vertex / m_baseVertex).min() };

	m_vertex = m_baseVertex * scale;
}