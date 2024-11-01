#include "Windows/Wrappers.h"

#include <wrl/client.h>
#include <d2d1_3.h>

#include "Plug.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

using namespace Voicemeeter::UI::D2D::Graphics::Glyphs;

Plug::Plug(
	Graphics::Canvas& canvas,
	const ::std::wstring& label
) : Glyph{ canvas, { 41, 19 } }
  , m_label{ label }
  , m_active{} {

}

void Plug::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	Glyph::Redraw(point, vertex);

	const Palette& palette{ m_canvas.get_Palette() };
	ID2D1SolidColorBrush* pBrush{
		(m_active
			? palette.get_pBrush(palette.get_Theme()
				.PrimaryActive)
			: palette.get_pBrush(palette.get_Theme()
				.Inactive))
	};
	bool fresh{ false };
	ID2D1PathGeometry* pTriangle{ palette.get_pGeometry(typeid(Triangle), fresh) };
	if (fresh) {
		::Microsoft::WRL::ComPtr<ID2D1GeometrySink> pSink{ nullptr };
		::Windows::ThrowIfFailed(pTriangle->Open(
			&pSink
		), "Geometry initialization failed");

		pSink->BeginFigure(
			::D2D1::Point2F(7.F, 3.5F),
			D2D1_FIGURE_BEGIN_FILLED
		);
		pSink->AddLine(
			::D2D1::Point2F(13.F, 9.F)
		);
		pSink->AddLine(
			::D2D1::Point2F(7.F, 15.5F)
		);
		pSink->EndFigure(
			D2D1_FIGURE_END_CLOSED
		);

		::Windows::ThrowIfFailed(pSink->Close(
		), "Geometry finalization failed");
	}
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
		->DrawRoundedRectangle(
			::D2D1::RoundedRect(::D2D1::RectF(0.75F, 0.75F, 40.25F, 18.25F), 6.25F, 6.25F),
			pBrush,
			1.5F);
	m_canvas.get_pRenderTarget()
		->FillGeometry(
			pTriangle,
			pBrush);
	m_canvas.get_pRenderTarget()
		->DrawTextLayout(
			::D2D1::Point2F(34.F - metrics.width, (19.F - metrics.height) / 2),
			pLayout,
			pBrush);
}