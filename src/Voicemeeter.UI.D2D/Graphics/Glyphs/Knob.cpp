#include "Windows/Wrappers.h"

#include "Knob.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

using namespace Voicemeeter::UI::D2D::Graphics::Glyphs;

Knob::Knob(
	Graphics::Canvas& canvas,
	const ::std::wstring& label
) : Glyph{ canvas, { 48, 48 } }
  , m_level{}
  , m_pinned{}
  , m_label{ label } {

}

void Knob::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	Glyph::Redraw(point, vertex);

	const Palette& palette{ m_canvas.get_Palette() };
	ID2D1SolidColorBrush* pBrush{
		(m_pinned
			? palette.get_pBrush(palette.get_Theme()
				.Warning)
			: 90.F < m_level
				? palette.get_pBrush(palette.get_Theme()
					.Danger)
				: palette.get_pBrush(palette.get_Theme()
					.PrimaryActive))
	};
	IDWriteTextLayout* pLayout{
		palette.get_pTextLayout(
			m_label,
			palette.get_Theme()
				.FontFamily
		)
	};
	DWRITE_TEXT_METRICS metrics{};
	::Windows::ThrowIfFailed(pLayout->GetMetrics(
		&metrics
	), "Text measurement failed");

	m_canvas.get_pRenderTarget()
		->DrawEllipse(
			::D2D1::Ellipse(::D2D1::Point2F(24.F, 24.F), 22.5F, 22.5F),
			pBrush,
			3.F);
	D2D1_MATRIX_3X2_F base{};
	double scale{ get_Size().x / get_BaseSize().x };
	m_canvas.get_pRenderTarget()
		->GetTransform(&base);
	m_canvas.get_pRenderTarget()
		->SetTransform(
			::D2D1::Matrix3x2F::Translation(24.F, 9.F)
			* ::D2D1::Matrix3x2F::Rotation(m_level, ::D2D1::Point2F(24.F, 24.F))
			* base);
	m_canvas.get_pRenderTarget()
		->FillEllipse(
			::D2D1::Ellipse(::D2D1::Point2F(0.F, 0.F), 2.75F, 2.75F),
			palette.get_pBrush(palette.get_Theme()
				.Indicator));
	m_canvas.get_pRenderTarget()
		->SetTransform(base);
	m_canvas.get_pRenderTarget()
		->DrawTextLayout(
			::D2D1::Point2F((48.F - metrics.width) / 2, (48.F - metrics.height) / 2),
			pLayout,
			pBrush);
};