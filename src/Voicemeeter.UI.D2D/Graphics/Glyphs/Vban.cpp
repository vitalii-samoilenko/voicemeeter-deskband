#include "Windows/Wrappers.h"

#include <wrl/client.h>
#include <d2d1_3.h>

#include "Vban.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

using namespace Voicemeeter::UI::D2D::Graphics::Glyphs;

Vban::Vban(
	Graphics::Canvas& canvas
) : Glyph{ canvas, { 39, 22 } }
  , m_label{ L"V" }
  , m_active{} {

}

void Vban::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	Glyph::Redraw(point, vertex);

	const Palette& palette{ m_canvas.get_Palette() };
	ID2D1SolidColorBrush* pBrush{
		(m_active
			? palette.get_pBrush(palette.get_Theme()
				.SecondaryActive)
			: palette.get_pBrush(palette.get_Theme()
				.Inactive))
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

	m_canvas.get_pD2dDeviceContext()
		->FillRectangle(
			::D2D1::RectF(0.F, 0.F, 7.5F, 22.F),
			pBrush);
	m_canvas.get_pD2dDeviceContext()
		->FillRectangle(
			::D2D1::RectF(31.5F, 0.F, 39.F, 22.F),
			pBrush);
	m_canvas.get_pD2dDeviceContext()
		->DrawRectangle(
			::D2D1::RectF(7.5F, 0.5F, 31.5F, 21.5F),
			pBrush);
	m_canvas.get_pD2dDeviceContext()
		->DrawTextLayout(
			::D2D1::Point2F((39.F - metrics.width) / 2, (22.F - metrics.height) / 2),
			pLayout,
			pBrush);
};