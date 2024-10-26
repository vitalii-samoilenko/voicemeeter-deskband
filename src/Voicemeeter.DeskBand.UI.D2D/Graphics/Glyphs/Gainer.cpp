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
			Brush());
	m_canvas.get_pRenderTarget()
		->FillEllipse(
			::D2D1::Ellipse(::D2D1::Point2F(m_level, 20.F), 20.F, 20.F),
			Brush());
};

ID2D1SolidColorBrush* Gainer::Brush() const {
	const ::std::string& key{
		"active"
	};
	ID2D1SolidColorBrush* pBrush{ nullptr };
	if (!m_canvas.get_pBrush(key, &pBrush)) {
		DWORD color{
			RGB(153, 195, 112)
		};
		pBrush->SetColor(::D2D1::ColorF(color));
	}
	return pBrush;
}