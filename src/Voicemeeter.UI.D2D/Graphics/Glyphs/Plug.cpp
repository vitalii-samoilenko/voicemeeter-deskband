#include "Windows/Wrappers.h"

#include <wrl/client.h>

#include "Plug.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

using namespace Voicemeeter::UI::D2D::Graphics::Glyphs;

Plug::Plug(
	Graphics::Canvas& canvas
) : Glyph{ canvas, { 41., 19. } }
  , m_label{}
  , m_color{ ::D2D1::ColorF(0.F, 0.F, 0.F, 0.F) } {

}

void Plug::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	Glyph::Redraw(point, vertex);

	struct Frame {};
	struct Triangle {};

	ID2D1SolidColorBrush* pBrush{ static_cast<ID2D1SolidColorBrush*>(
		get_Canvas()
			.get_Palette()
				.get_pBrush(typeid(Frame),
					[this](ID2D1Brush** ppBrush)->void {
						ID2D1SolidColorBrush* pBrush{ nullptr };
						::Windows::ThrowIfFailed(get_Canvas()
							.get_pD2dDeviceContext()
								->CreateSolidColorBrush(
									m_color,
									&pBrush
						), "Brush creation failed");
						*ppBrush = pBrush;
					})) };
	pBrush->SetColor(m_color);
	ID2D1GeometryRealization* pFrame{ get_Canvas()
		.get_Palette()
			.get_pGeometry(typeid(Frame),
		[this](ID2D1GeometryRealization** ppGeometry, FLOAT flatteringTolerance)->void {
			::Microsoft::WRL::ComPtr<ID2D1RoundedRectangleGeometry> pRectangle{ nullptr };
			::Windows::ThrowIfFailed(get_Canvas()
				.get_pD2dFactory()
					->CreateRoundedRectangleGeometry(
						::D2D1::RoundedRect(::D2D1::RectF(0.75F, 0.75F, 40.25F, 18.25F), 6.25F, 6.25F),
						&pRectangle
			), "Rectangle creation failed");

			::Windows::ThrowIfFailed(get_Canvas()
				.get_pD2dDeviceContext()
					->CreateStrokedGeometryRealization(
						pRectangle.Get(), flatteringTolerance, 1.5F, nullptr,
						ppGeometry
			), "Geometry creation failed");
		}) };
	ID2D1GeometryRealization* pTriangle{ get_Canvas()
		.get_Palette()
			.get_pGeometry(typeid(Triangle),
		[this](ID2D1GeometryRealization** ppGeometry, FLOAT flatteringTolerance)->void {
			::Microsoft::WRL::ComPtr<ID2D1PathGeometry> pPath{ nullptr };
			::Windows::ThrowIfFailed(get_Canvas()
				.get_pD2dFactory()
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

			::Windows::ThrowIfFailed(get_Canvas()
				.get_pD2dDeviceContext()
					->CreateFilledGeometryRealization(
						pPath.Get(), flatteringTolerance,
						ppGeometry
			), "Geometry creation failed");
		}) };
	IDWriteTextLayout* pLayout{ get_Canvas()
		.get_Palette()
			.get_pTextLayout(
				m_label,
				get_Canvas()
					.get_Palette()
						.get_Theme()
							.FontFamily)
	};
	DWRITE_TEXT_METRICS metrics{};
	::Windows::ThrowIfFailed(pLayout->GetMetrics(
		&metrics
	), "Text measurement failed");

	get_Canvas()
		.get_pD2dDeviceContext()
			->DrawGeometryRealization(
				pFrame,
				pBrush);
	get_Canvas()
		.get_pD2dDeviceContext()
			->DrawGeometryRealization(
				pTriangle,
				pBrush);
	get_Canvas()
		.get_pD2dDeviceContext()
			->DrawTextLayout(
				::D2D1::Point2F(34.F - metrics.width, (19.F - metrics.height) / 2),
				pLayout,
				pBrush);
}