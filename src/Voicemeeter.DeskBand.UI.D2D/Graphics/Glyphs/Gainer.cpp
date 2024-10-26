#include "Gainer.h"

using namespace Voicemeeter::DeskBand::UI::D2D::Graphics::Glyphs;

Gainer::Gainer(
	Graphics::Canvas& canvas
) : Glyph{ canvas, { 165, 40 } }
  , m_level{} {

}

void Gainer::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	Glyph::Redraw(point, vertex);

	m_canvas.get_pRenderTarget()
		->FillRoundedRectangle(
			::D2D1::RoundedRect(::D2D1::RectF(0.F, 11.F, 165.F, 29.F), 8.F, 8.F),
			m_canvas.get_pBrush(RGB(112, 195, 153)));
	m_canvas.get_pRenderTarget()
		->FillEllipse(
			::D2D1::Ellipse(::D2D1::Point2F(m_level, 20.F), 20.F, 20.F),
			m_canvas.get_pBrush(RGB(112, 195, 153)));
};