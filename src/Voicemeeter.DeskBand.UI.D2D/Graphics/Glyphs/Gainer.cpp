#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "Gainer.h"

using namespace ::Voicemeeter::DeskBand::Windows;

using namespace Voicemeeter::DeskBand::UI::D2D::Graphics::Glyphs;

Gainer::Gainer(
	Graphics::Canvas& canvas
) : Glyph{ canvas, { 165, 40 } }
  , m_level{} {

}

void Gainer::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	Glyph::Redraw(point, vertex);

	m_canvas.get_pRenderTarget()
		->DrawRoundedRectangle(
			::D2D1::RoundedRect(::D2D1::RectF(0.75F, 11.75F, 164.25F, 28.25F), 7.25F, 7.25F),
			Brush(),
			1.5F);
	m_canvas.get_pRenderTarget()
		->FillRoundedRectangle(
			::D2D1::RoundedRect(::D2D1::RectF(0.F, 11.F, 28.0F, 29.F), 8.F, 8.F),
			Brush());
	m_canvas.get_pRenderTarget()
		->FillRectangle(
			::D2D1::Rect(24.F, 11.F, m_level, 29.F),
			Brush());
	D2D1_MATRIX_3X2_F base{};
	double scale{ get_Size().x / get_BaseSize().x };
	m_canvas.get_pRenderTarget()
		->GetTransform(&base);
	m_canvas.get_pRenderTarget()
		->SetTransform(::D2D1::Matrix3x2F::Rotation(-45.F, ::D2D1::Point2F(0.F, 20.F))
			* base
			* ::D2D1::Matrix3x2F::Translation(static_cast<FLOAT>(m_level * scale), 0.F));
	m_canvas.get_pRenderTarget()
		->FillGeometry(
			HalfCircle(),
			Light());
	m_canvas.get_pRenderTarget()
		->SetTransform(::D2D1::Matrix3x2F::Rotation(135.F, ::D2D1::Point2F(0.F, 20.F))
			* base
			* ::D2D1::Matrix3x2F::Translation(static_cast<FLOAT>(m_level * scale), 0.F));
	m_canvas.get_pRenderTarget()
		->FillGeometry(
			HalfCircle(),
			Dark());
	m_canvas.get_pRenderTarget()
		->SetTransform(base);
	m_canvas.get_pRenderTarget()
		->DrawEllipse(
			::D2D1::Ellipse(::D2D1::Point2F(m_level, 20.F), 18.75F, 18.75F),
			Brush(),
			2.5F);
	m_canvas.get_pRenderTarget()
		->FillEllipse(
			::D2D1::Ellipse(::D2D1::Point2F(m_level, 20.F), 2.75F, 2.75F),
			White());
};

ID2D1SolidColorBrush* Gainer::Brush() const {
	const ::std::string& key{
		118.F < m_level
			? "danger"
			: "active"
	};
	ID2D1SolidColorBrush* pBrush{ nullptr };
	if (!m_canvas.get_pBrush(key, &pBrush)) {
		DWORD color{
			118.F < m_level
				? RGB(77, 99, 248)
				: RGB(153, 195, 112)
		};
		pBrush->SetColor(::D2D1::ColorF(color));
	}
	return pBrush;
}
ID2D1SolidColorBrush* Gainer::Light() const {
	const ::std::string& key{
		"gainer_light"
	};
	ID2D1SolidColorBrush* pBrush{ nullptr };
	if (!m_canvas.get_pBrush(key, &pBrush)) {
		pBrush->SetColor(::D2D1::ColorF(1.F, 1.F, 1.F, 0.4F));
	}
	return pBrush;
}
ID2D1SolidColorBrush* Gainer::Dark() const {
	const ::std::string& key{
		"gainer_dark"
	};
	ID2D1SolidColorBrush* pBrush{ nullptr };
	if (!m_canvas.get_pBrush(key, &pBrush)) {
		pBrush->SetColor(::D2D1::ColorF(0.75F, 0.75F, 0.75F, 0.4F));
	}
	return pBrush;
}
ID2D1SolidColorBrush* Gainer::White() const {
	const ::std::string& key{
		"white"
	};
	ID2D1SolidColorBrush* pBrush{ nullptr };
	if (!m_canvas.get_pBrush(key, &pBrush)) {
		pBrush->SetColor(::D2D1::ColorF(1.F, 1.F, 1.F));
	}
	return pBrush;
};
ID2D1PathGeometry* Gainer::HalfCircle() const {
	const ::std::string& key{
		"gainer_half_circle"
	};
	ID2D1PathGeometry* pHalfCircle{ nullptr };
	if (!m_canvas.get_pGeometry(key, &pHalfCircle)) {
		::Microsoft::WRL::ComPtr<ID2D1GeometrySink> pSink{ nullptr };
		ThrowIfFailed(pHalfCircle->Open(
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

		ThrowIfFailed(pSink->Close(
		), "Geometry finalization failed");
	}
	return pHalfCircle;
}