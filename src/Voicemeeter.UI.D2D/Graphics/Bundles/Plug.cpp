#include "Windows/Wrappers.h"

#include "Plug.h"

using namespace Voicemeeter::UI::D2D::Graphics::Bundles;

Plug::Plug(
	Graphics::Palette& palette,
	const ::std::wstring& label
) : Bundle{ palette, { 41., 19. } }
  , m_label{ label }
  , m_color{
		get_Palette()
			.get_Theme()
				.Inactive
  } {

}

void Plug::Execute() {
	Bundle::Execute();

	D2D1_MATRIX_3X2_F base{};
	get_Palette()
		.get_pD2dDeviceContext()
			->GetTransform(&base);

	struct Frame {};
	struct Triangle {};

	ID2D1SolidColorBrush* pBrush{
		static_cast<ID2D1SolidColorBrush*>(
			get_Palette()
				.get_pBrush<Frame>(
					[this](ID2D1Brush** ppBrush)->void {
						ID2D1SolidColorBrush* pBrush{ nullptr };
						::Windows::ThrowIfFailed(get_Palette()
							.get_pD2dDeviceContext()
								->CreateSolidColorBrush(
									m_color,
									&pBrush
						), "Brush creation failed");
						*ppBrush = pBrush;
					}))
	};
	pBrush->SetColor(m_color);
	//ID2D1GeometryRealization* pFrame{
	//	get_Palette()
	//		.get_pGeometry<Frame>(
	//			[this, &base](ID2D1GeometryRealization** ppGeometry)->void {
	//				::Microsoft::WRL::ComPtr<ID2D1RoundedRectangleGeometry> pRectangle{ nullptr };
	//				::Windows::ThrowIfFailed(get_Palette()
	//					.get_pD2dFactory()
	//						->CreateRoundedRectangleGeometry(
	//							::D2D1::RoundedRect(::D2D1::RectF(0.75F, 0.75F, 40.25F, 18.25F), 6.25F, 6.25F),
	//							&pRectangle
	//				), "Rectangle creation failed");

	//				::Windows::ThrowIfFailed(get_Palette()
	//					.get_pD2dDeviceContext()
	//						->CreateStrokedGeometryRealization(
	//							pRectangle.Get(), ::D2D1::ComputeFlatteningTolerance(base), 1.5F, nullptr,
	//							ppGeometry
	//				), "Geometry creation failed");
	//			})
	//};
	//ID2D1GeometryRealization* pTriangle{
	//	get_Palette()
	//		.get_pGeometryRealization<Triangle>(
	//			[this, &base](ID2D1GeometryRealization** ppGeometry)->void {
	//				::Microsoft::WRL::ComPtr<ID2D1PathGeometry> pPath{ nullptr };
	//				::Windows::ThrowIfFailed(get_Palette()
	//					.get_pD2dFactory()
	//						->CreatePathGeometry(
	//							&pPath
	//				), "Path creation failed");
	//				::Microsoft::WRL::ComPtr<ID2D1GeometrySink> pSink{ nullptr };
	//				::Windows::ThrowIfFailed(pPath->Open(
	//					&pSink
	//				), "Path initialization failed");

	//				pSink->BeginFigure(
	//					::D2D1::Point2F(7.F, 3.5F),
	//					D2D1_FIGURE_BEGIN_FILLED
	//				);
	//				pSink->AddLine(
	//					::D2D1::Point2F(13.F, 9.F)
	//				);
	//				pSink->AddLine(
	//					::D2D1::Point2F(7.F, 15.5F)
	//				);
	//				pSink->EndFigure(
	//					D2D1_FIGURE_END_CLOSED
	//				);

	//				::Windows::ThrowIfFailed(pSink->Close(
	//				), "Path finalization failed");

	//				::Windows::ThrowIfFailed(get_Palette()
	//					.get_pD2dDeviceContext()
	//						->CreateFilledGeometryRealization(
	//							pPath.Get(), ::D2D1::ComputeFlatteningTolerance(base),
	//							ppGeometry
	//				), "Geometry creation failed");
	//			})
	//};
	ID2D1Geometry* pTriangle{
		get_Palette()
			.get_pGeometry<Triangle>(
				[this](ID2D1Geometry** ppGeometry)->void {
					::Microsoft::WRL::ComPtr<ID2D1PathGeometry> pPath{ nullptr };
					::Windows::ThrowIfFailed(get_Palette()
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

					*ppGeometry = pPath.Get();
					pPath.Detach();
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
			->DrawRoundedRectangle(
				::D2D1::RoundedRect(::D2D1::RectF(0.75F, 0.75F, 40.25F, 18.25F), 6.25F, 6.25F),
				pBrush, 1.5F);
	get_Palette()
		.get_pD2dDeviceContext()
			->FillGeometry(
				pTriangle,
				pBrush);
	get_Palette()
		.get_pD2dDeviceContext()
			->DrawTextLayout(
				::D2D1::Point2F(34.F - metrics.width, (19.F - metrics.height) / 2),
				pLayout,
				pBrush);
}