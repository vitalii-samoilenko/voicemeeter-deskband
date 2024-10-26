#include "Gainer.h"

using namespace Voicemeeter::DeskBand::UI::Graphics::D2D::Glyphs;

Gainer::Gainer(
	Canvas& canvas
) : Glyph{ canvas, { 165, 40 } }
  , m_level{} {

}

void Gainer::OnDraw(const D2D::Canvas& canvas, const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) const {
	canvas.get_pRenderTarget()
		->FillRectangle(
			::D2D1::RectF(0.F, 0.F, 165.F, 40.F),
			canvas.get_pBackgroundBrush());
	canvas.get_pRenderTarget()
		->FillRoundedRectangle(
			::D2D1::RoundedRect(::D2D1::RectF(0.F, 11.F, 165.F, 29.F), 8.F, 8.F),
			canvas.get_pBrush(RGB(112, 195, 153)));
	canvas.get_pRenderTarget()
		->FillEllipse(
			::D2D1::Ellipse(::D2D1::Point2F(28.F + m_level * 1.5F, 20.F), 20.F, 20.F),
			canvas.get_pBrush(RGB(112, 195, 153)));
};