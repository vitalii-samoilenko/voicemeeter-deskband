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
	struct Frame {};
	ID2D1GeometryRealization* pFrame{ palette.get_pGeometry(typeid(Frame),
		[this](ID2D1GeometryRealization** ppGeometry, FLOAT flatteringTolerance)->void {
			::Microsoft::WRL::ComPtr<ID2D1RoundedRectangleGeometry> pRectangle{ nullptr };
			::Windows::ThrowIfFailed(m_canvas.get_pD2dFactory()
				->CreateRoundedRectangleGeometry(
					::D2D1::RoundedRect(::D2D1::RectF(0.75F, 0.75F, 40.25F, 18.25F), 6.25F, 6.25F),
					&pRectangle
			), "Rectangle creation failed");

			::Windows::ThrowIfFailed(m_canvas.get_pD2dDeviceContext()
				->CreateStrokedGeometryRealization(
					pRectangle.Get(), flatteringTolerance, 1.5F, nullptr,
					ppGeometry
			), "Geometry creation failed");
		}) };
	struct Triangle {};
	ID2D1GeometryRealization* pTriangle{ palette.get_pGeometry(typeid(Triangle),
		[this](ID2D1GeometryRealization** ppGeometry, FLOAT flatteringTolerance)->void {
			::Microsoft::WRL::ComPtr<ID2D1PathGeometry> pPath{ nullptr };
			::Windows::ThrowIfFailed(m_canvas.get_pD2dFactory()
				->CreatePathGeometry(
					&pPath
			), "Path creation failed");
			::Microsoft::WRL::ComPtr<ID2D1GeometrySink> pSink{ nullptr };
			::Windows::ThrowIfFailed(pPath->Open(
				&pSink
			), "Path initialization failed");

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
			), "Path finalization failed");

			::Windows::ThrowIfFailed(m_canvas.get_pD2dDeviceContext()
				->CreateFilledGeometryRealization(
					pPath.Get(), flatteringTolerance,
					ppGeometry
			), "Geometry creation failed");
		}) };
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
		->DrawGeometryRealization(
			pFrame,
			pBrush);
	m_canvas.get_pD2dDeviceContext()
		->DrawGeometryRealization(
			pTriangle,
			pBrush);
	m_canvas.get_pD2dDeviceContext()
		->DrawTextLayout(
			::D2D1::Point2F(34.F - metrics.width, (19.F - metrics.height) / 2),
			pLayout,
			pBrush);
}