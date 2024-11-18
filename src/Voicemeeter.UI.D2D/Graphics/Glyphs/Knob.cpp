#include <cmath>
#include <string>

#include "Windows/Wrappers.h"

#include <wrl/client.h>

#include "Knob.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

using namespace Voicemeeter::UI::D2D::Graphics::Glyphs;

Knob::Knob(
	Graphics::Canvas& canvas
) : Glyph{ canvas, { 48., 48. } }
  , m_label{}
  , m_color{ ::D2D1::ColorF(0.F, 0.F, 0.F, 0.F) }
  , m_angle{} {

}

void Knob::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	Glyph::Redraw(point, vertex);

	struct Frame {};
	struct Indicator {};

	const Palette& palette{ m_canvas.get_Palette() };
	ID2D1SolidColorBrush* pFrameBrush{ static_cast<ID2D1SolidColorBrush*>(
		palette.get_pBrush(typeid(Frame),
			[this](ID2D1Brush** ppBrush)->void {
				ID2D1SolidColorBrush* pBrush{ nullptr };
				::Windows::ThrowIfFailed(m_canvas.get_pD2dDeviceContext()
					->CreateSolidColorBrush(
						m_color,
						&pBrush
				), "Brush creation failed");
				*ppBrush = pBrush;
			})) };
	pFrameBrush->SetColor(m_color);
	ID2D1SolidColorBrush* pIndicatorBrush{ static_cast<ID2D1SolidColorBrush*>(
		palette.get_pBrush(typeid(Frame),
			[this](ID2D1Brush** ppBrush)->void {
				ID2D1SolidColorBrush* pBrush{ nullptr };
				::Windows::ThrowIfFailed(m_canvas.get_pD2dDeviceContext()
					->CreateSolidColorBrush(
						m_canvas.get_Palette()
							.get_Theme()
								.Indicator,
						&pBrush
				), "Brush creation failed");
				*ppBrush = pBrush;
			})) };
	ID2D1GeometryRealization* pFrame{ palette.get_pGeometry(typeid(Frame),
		[this](ID2D1GeometryRealization** ppGeometry, FLOAT flatteringTolerance)->void {
			::Microsoft::WRL::ComPtr<ID2D1EllipseGeometry> pEllipse{ nullptr };
			::Windows::ThrowIfFailed(m_canvas.get_pD2dFactory()
				->CreateEllipseGeometry(
					::D2D1::Ellipse(::D2D1::Point2F(24.F, 24.F), 22.5F, 22.5F),
					&pEllipse
			), "Ellipse creation failed");

			::Windows::ThrowIfFailed(m_canvas.get_pD2dDeviceContext()
				->CreateStrokedGeometryRealization(
					pEllipse.Get(), flatteringTolerance, 3.F, nullptr,
					ppGeometry
			), "Geometry creation failed");
		}) };
	ID2D1GeometryRealization* pIndicator{ palette.get_pGeometry(typeid(Indicator),
		[this](ID2D1GeometryRealization** ppGeometry, FLOAT flatteringTolerance)->void {
			::Microsoft::WRL::ComPtr<ID2D1EllipseGeometry> pEllipse{ nullptr };
			::Windows::ThrowIfFailed(m_canvas.get_pD2dFactory()
				->CreateEllipseGeometry(
					::D2D1::Ellipse(::D2D1::Point2F(0.F, 0.F), 2.75F, 2.75F),
					&pEllipse
			), "Ellipse creation failed");

			::Windows::ThrowIfFailed(m_canvas.get_pD2dDeviceContext()
				->CreateFilledGeometryRealization(
					pEllipse.Get(), flatteringTolerance,
					ppGeometry
			), "Geometry creation failed");
		}) };
	IDWriteTextLayout* pLayout{
		palette.get_pTextLayout(
			m_label,
			palette.get_Theme()
				.FontFamily)
	};
	DWRITE_TEXT_METRICS metrics{};
	::Windows::ThrowIfFailed(pLayout->GetMetrics(
		&metrics
	), "Text measurement failed");

	m_canvas.get_pD2dDeviceContext()
		->DrawTextLayout(
			::D2D1::Point2F((48.F - metrics.width) / 2, (48.F - metrics.height) / 2),
			pLayout,
			pFrameBrush);
	m_canvas.get_pD2dDeviceContext()
		->DrawGeometryRealization(
			pFrame,
			pFrameBrush);
	D2D1_MATRIX_3X2_F base{};
	double scale{ get_Size()[0] / get_BaseSize()[0] };
	m_canvas.get_pD2dDeviceContext()
		->GetTransform(&base);
	m_canvas.get_pD2dDeviceContext()
		->SetTransform(
			::D2D1::Matrix3x2F::Translation(24.F, 9.F)
			* ::D2D1::Matrix3x2F::Rotation(m_angle, ::D2D1::Point2F(24.F, 24.F))
			* base);
	m_canvas.get_pD2dDeviceContext()
		->DrawGeometryRealization(
			pIndicator,
			pIndicatorBrush);
	m_canvas.get_pD2dDeviceContext()
		->SetTransform(base);
};