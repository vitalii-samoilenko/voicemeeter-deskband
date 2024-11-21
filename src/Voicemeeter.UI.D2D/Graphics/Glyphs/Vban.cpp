#include "Windows/Wrappers.h"

#include <wrl/client.h>

#include "Vban.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

using namespace Voicemeeter::UI::D2D::Graphics::Glyphs;

Vban::Vban(
	Graphics::Canvas& canvas
) : Glyph{ canvas, { 39., 22. } }
  , m_color{ ::D2D1::ColorF(0.F, 0.F, 0.F, 0.F) } {

}

void Vban::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	Glyph::Redraw(point, vertex);

	struct Frame {};
	struct FrameSide {};

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
			::Microsoft::WRL::ComPtr<ID2D1RectangleGeometry> pRectangle{ nullptr };
			::Windows::ThrowIfFailed(get_Canvas()
				.get_pD2dFactory()
					->CreateRectangleGeometry(
						::D2D1::RectF(7.5F, 0.5F, 31.5F, 21.5F),
						&pRectangle
			), "Rectangle creation failed");

			::Windows::ThrowIfFailed(get_Canvas()
				.get_pD2dDeviceContext()
					->CreateStrokedGeometryRealization(
						pRectangle.Get(), flatteringTolerance, 1.F, nullptr,
						ppGeometry
			), "Geometry creation failed");
		}) };
	ID2D1GeometryRealization* pFrameSide{ get_Canvas()
		.get_Palette()
			.get_pGeometry(typeid(FrameSide),
		[this](ID2D1GeometryRealization** ppGeometry, FLOAT flatteringTolerance)->void {
			::Microsoft::WRL::ComPtr<ID2D1RectangleGeometry> pRectangle{ nullptr };
			::Windows::ThrowIfFailed(get_Canvas()
				.get_pD2dFactory()
					->CreateRectangleGeometry(
						::D2D1::RectF(0.F, 0.F, 7.5F, 22.F),
						&pRectangle
			), "Rectangle creation failed");

			::Windows::ThrowIfFailed(get_Canvas()
				.get_pD2dDeviceContext()
					->CreateFilledGeometryRealization(
						pRectangle.Get(), flatteringTolerance,
						ppGeometry
			), "Geometry creation failed");
		}) };
	IDWriteTextLayout* pLayout{ get_Canvas()
		.get_Palette()
			.get_pTextLayout(
				L"V",
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
			->DrawTextLayout(
				::D2D1::Point2F((39.F - metrics.width) / 2, (22.F - metrics.height) / 2),
				pLayout,
				pBrush);
	get_Canvas()
		.get_pD2dDeviceContext()
			->DrawGeometryRealization(
				pFrameSide,
				pBrush);
	D2D1_MATRIX_3X2_F base{};
	get_Canvas()
		.get_pD2dDeviceContext()
			->GetTransform(&base);
	get_Canvas()
		.get_pD2dDeviceContext()
			->SetTransform(
				::D2D1::Matrix3x2F::Translation(31.5F, 0.F)
				* base);
	get_Canvas()
		.get_pD2dDeviceContext()
			->DrawGeometryRealization(
				pFrameSide,
				pBrush);
	get_Canvas()
		.get_pD2dDeviceContext()
			->SetTransform(base);
};