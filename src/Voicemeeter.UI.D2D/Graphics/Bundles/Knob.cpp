#include "Windows/Wrappers.h"

#include "Knob.h"

using namespace Voicemeeter::UI::D2D::Graphics::Bundles;

Knob::Knob(
	Graphics::Palette& palette,
	const ::std::wstring& label
) : Bundle{ palette, { 48., 48. } }
  , m_label{ label }
  , m_frameColor{
		get_Palette()
			.get_Theme()
				.Inactive
	}
  , m_labelColor{
		get_Palette()
			.get_Theme()
				.Inactive
	}
  , m_angle{ 90.F } {

}

void Knob::Execute() {
	Bundle::Execute();

	struct Frame {};
	struct Label {};
	struct Indicator {};

	ID2D1SolidColorBrush* pFrameBrush{
		static_cast<ID2D1SolidColorBrush*>(
			get_Palette()
				.get_pBrush<Frame>(
					[this](ID2D1Brush** ppBrush)->void {
						ID2D1SolidColorBrush* pBrush{ nullptr };
						::Windows::ThrowIfFailed(get_Palette()
							.get_pD2dDeviceContext()
								->CreateSolidColorBrush(
									m_frameColor,
									&pBrush
						), "Brush creation failed");
						*ppBrush = pBrush;
					}))
	};
	pFrameBrush->SetColor(m_frameColor);
	ID2D1SolidColorBrush* pLabelBrush{
		static_cast<ID2D1SolidColorBrush*>(
			get_Palette()
				.get_pBrush<Label>(
					[this](ID2D1Brush** ppBrush)->void {
						ID2D1SolidColorBrush* pBrush{ nullptr };
						::Windows::ThrowIfFailed(get_Palette()
							.get_pD2dDeviceContext()
								->CreateSolidColorBrush(
									m_labelColor,
									&pBrush
						), "Brush creation failed");
						*ppBrush = pBrush;
					}))
	};
	pLabelBrush->SetColor(m_labelColor);
	ID2D1SolidColorBrush* pIndicatorBrush{
		static_cast<ID2D1SolidColorBrush*>(
			get_Palette()
				.get_pBrush<Indicator>(
					[this](ID2D1Brush** ppBrush)->void {
						ID2D1SolidColorBrush* pBrush{ nullptr };
						::Windows::ThrowIfFailed(get_Palette()
							.get_pD2dDeviceContext()
								->CreateSolidColorBrush(
									get_Palette()
										.get_Theme()
											.Indicator,
									&pBrush
						), "Brush creation failed");
						*ppBrush = pBrush;
					}))
	};
	ID2D1GeometryRealization* pFrame{
		get_Palette()
			.get_pGeometry<Frame>(
				[this](ID2D1GeometryRealization** ppGeometry, FLOAT flatteringTolerance)->void {
					::Microsoft::WRL::ComPtr<ID2D1EllipseGeometry> pEllipse{ nullptr };
					::Windows::ThrowIfFailed(get_Palette()
						.get_pD2dFactory()
							->CreateEllipseGeometry(
								::D2D1::Ellipse(::D2D1::Point2F(24.F, 24.F), 22.5F, 22.5F),
								&pEllipse
					), "Ellipse creation failed");

					::Windows::ThrowIfFailed(get_Palette()
						.get_pD2dDeviceContext()
							->CreateStrokedGeometryRealization(
								pEllipse.Get(), flatteringTolerance, 3.F, nullptr,
								ppGeometry
					), "Geometry creation failed");
				})
	};
	ID2D1GeometryRealization* pIndicator{
		get_Palette()
			.get_pGeometry<Indicator>(
				[this](ID2D1GeometryRealization** ppGeometry, FLOAT flatteringTolerance)->void {
					::Microsoft::WRL::ComPtr<ID2D1EllipseGeometry> pEllipse{ nullptr };
					::Windows::ThrowIfFailed(get_Palette()
						.get_pD2dFactory()
							->CreateEllipseGeometry(
								::D2D1::Ellipse(::D2D1::Point2F(0.F, 0.F), 2.75F, 2.75F),
								&pEllipse
					), "Ellipse creation failed");

					::Windows::ThrowIfFailed(get_Palette()
						.get_pD2dDeviceContext()
							->CreateFilledGeometryRealization(
								pEllipse.Get(), flatteringTolerance,
								ppGeometry
					), "Geometry creation failed");
				})
	};
	IDWriteTextLayout* pLayout{
		get_Palette()
			.get_pTextLayout(
				m_label,
				get_Palette()
					.get_Theme()
						.FontFamily)
	};
	DWRITE_TEXT_METRICS metrics{};
	::Windows::ThrowIfFailed(pLayout->GetMetrics(
		&metrics
	), "Text measurement failed");

	get_Palette()
		.get_pD2dDeviceContext()
			->DrawTextLayout(
				::D2D1::Point2F((48.F - metrics.width) / 2, (48.F - metrics.height) / 2),
				pLayout,
				pLabelBrush);
	get_Palette()
		.get_pD2dDeviceContext()
			->DrawGeometryRealization(
				pFrame,
				pFrameBrush);
	D2D1_MATRIX_3X2_F base{};
	double scale{ get_Size()[0] / get_BaseSize()[0] };
	get_Palette()
		.get_pD2dDeviceContext()
			->GetTransform(&base);
	get_Palette()
		.get_pD2dDeviceContext()
			->SetTransform(
				::D2D1::Matrix3x2F::Translation(24.F, 9.F)
				* ::D2D1::Matrix3x2F::Rotation(m_angle, ::D2D1::Point2F(24.F, 24.F))
				* base);
	get_Palette()
		.get_pD2dDeviceContext()
			->DrawGeometryRealization(
				pIndicator,
				pIndicatorBrush);
	get_Palette()
		.get_pD2dDeviceContext()
			->SetTransform(base);
};