#include "Windows/Wrappers.h"

#include "Gainer.h"

#pragma comment(lib, "d2d1")

using namespace Voicemeeter::UI::D2D::Graphics::Glyphs;

Gainer::Gainer(
	Graphics::Canvas& canvas
) : Glyph{ canvas, { 165, 40 } }
  , m_level{} {

}

struct HalfCircle {};

void Gainer::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	Glyph::Redraw(point, vertex);

	const Palette& palette{ m_canvas.get_Palette() };
	ID2D1SolidColorBrush* pBrush{
		(118.F < m_level
			? palette.get_pBrush(palette.get_Theme()
				.Danger)
			: palette.get_pBrush(palette.get_Theme()
				.PrimaryActive))
	};
	bool fresh{ false };
	ID2D1PathGeometry* pHalfCircle{ palette.get_pGeometry(typeid(HalfCircle), fresh) };
	if (fresh) {
		::Microsoft::WRL::ComPtr<ID2D1GeometrySink> pSink{ nullptr };
		::Windows::ThrowIfFailed(pHalfCircle->Open(
			&pSink
		), "Geometry initialization failed");

		pSink->BeginFigure(
			::D2D1::Point2F(0.F, 0.F),
			D2D1_FIGURE_BEGIN_FILLED
		);
		pSink->AddArc(
			::D2D1::ArcSegment(
				::D2D1::Point2F(0.F, 40.F),
				::D2D1::Size(20.F, 20.F),
				180.F,
				D2D1_SWEEP_DIRECTION_CLOCKWISE,
				D2D1_ARC_SIZE_SMALL
			)
		);
		pSink->EndFigure(
			D2D1_FIGURE_END_CLOSED
		);

		::Windows::ThrowIfFailed(pSink->Close(
		), "Geometry finalization failed");
	}

	m_canvas.get_pD2dDeviceContext()
		->DrawRoundedRectangle(
			::D2D1::RoundedRect(::D2D1::RectF(0.75F, 11.75F, 164.25F, 28.25F), 7.25F, 7.25F),
			pBrush,
			1.5F);
	m_canvas.get_pD2dDeviceContext()
		->FillRoundedRectangle(
			::D2D1::RoundedRect(::D2D1::RectF(0.F, 11.F, 28.0F, 29.F), 8.F, 8.F),
			pBrush);
	m_canvas.get_pD2dDeviceContext()
		->FillRectangle(
			::D2D1::Rect(24.F, 11.F, m_level, 29.F),
			pBrush);
	D2D1_MATRIX_3X2_F base{};
	double scale{ get_Size().x / get_BaseSize().x };
	m_canvas.get_pD2dDeviceContext()
		->GetTransform(&base);
	m_canvas.get_pD2dDeviceContext()
		->SetTransform(
			::D2D1::Matrix3x2F::Rotation(-45.F, ::D2D1::Point2F(0.F, 20.F))
			* base
			* ::D2D1::Matrix3x2F::Translation(static_cast<FLOAT>(m_level * scale), 0.F));
	m_canvas.get_pD2dDeviceContext()
		->FillGeometry(
			pHalfCircle,
			palette.get_pBrush(palette.get_Theme()
				.LightGlass));
	m_canvas.get_pD2dDeviceContext()
		->SetTransform(
			::D2D1::Matrix3x2F::Rotation(135.F, ::D2D1::Point2F(0.F, 20.F))
			* base
			* ::D2D1::Matrix3x2F::Translation(static_cast<FLOAT>(m_level * scale), 0.F));
	m_canvas.get_pD2dDeviceContext()
		->FillGeometry(
			pHalfCircle,
			palette.get_pBrush(palette.get_Theme()
				.DarkGlass));
	m_canvas.get_pD2dDeviceContext()
		->SetTransform(base);
	m_canvas.get_pD2dDeviceContext()
		->DrawEllipse(
			::D2D1::Ellipse(::D2D1::Point2F(m_level, 20.F), 18.75F, 18.75F),
			pBrush,
			2.5F);
	m_canvas.get_pD2dDeviceContext()
		->FillEllipse(
			::D2D1::Ellipse(::D2D1::Point2F(m_level, 20.F), 2.75F, 2.75F),
			palette.get_pBrush(palette.get_Theme()
				.Indicator));
};