#include <algorithm>

#include <d2d1_3.h>
#pragma comment(lib, "d2d1")

#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "Glyph.h"

using namespace ::Voicemeeter::DeskBand::Windows;
using namespace ::Voicemeeter::DeskBand::UI::Graphics::D2D;

Glyph::Glyph(
	Canvas& canvas,
	const ::linear_algebra::vector& baseVertex
) : m_canvas{ canvas }
  , m_point{}
  , m_vertex{ baseVertex }
  , m_baseVertex{ baseVertex } {

}

const ::linear_algebra::vector& Glyph::get_Position() const {
	return m_point;
}
const ::linear_algebra::vector& Glyph::get_Size() const {
	return m_vertex;
}
const ::linear_algebra::vector& Glyph::get_BaseSize() const {
	return m_baseVertex;
}

void Glyph::Redraw(const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) {
	ID2D1HwndRenderTarget* pRenderTarget{ m_canvas.get_pRenderTarget() };

	pRenderTarget->BeginDraw();

	pRenderTarget->SetTransform(
		::D2D1::Matrix3x2F::Scale(static_cast<FLOAT>(m_vertex.x) / m_baseVertex.x, static_cast<FLOAT>(m_vertex.y) / m_baseVertex.y)
		* ::D2D1::Matrix3x2F::Translation(m_point.x, m_point.y));
	OnDraw(point - m_point, vertex);
	pRenderTarget->SetTransform(
		::D2D1::Matrix3x2F::Identity());

	ThrowIfFailed(pRenderTarget->EndDraw(
	), "Render failed");
};

void Glyph::Move(const ::linear_algebra::vector& point) {
	m_point = point;
}

void Glyph::Rescale(const ::linear_algebra::vector& vertex) {
	double scale{
		::std::min<double>(
			static_cast<double>(vertex.x) / m_baseVertex.x,
			static_cast<double>(vertex.y) / m_baseVertex.y)
	};

	m_vertex.x = m_baseVertex.x * scale;
	m_vertex.y = m_baseVertex.y * scale;
};