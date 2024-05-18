#include "sprite.h"

#include <wrl/client.h>
#include <wincodec.h>
#include <d2d1_3.h>
#pragma comment(lib, "d2d1")
#include <dwrite_3.h>
#pragma comment(lib, "dwrite")

#include "../wrappers.h"

using namespace Voicemeeter::DeskBand::Windows::Presentation;

using Microsoft::WRL::ComPtr;

static constexpr UINT SPRITE_WIDTH{ 230U };
static constexpr UINT SPRITE_HEGHT{ 184U };
static D2D1_ELLIPSE out{ D2D1::Ellipse(D2D1::Point2F(23.F, 23.F), 23.F, 23.F) };
static FLOAT stroke{ 2.F };

Sprite::Region Sprite::get_Region(Sprite_element element, size_t mipmap) const {
	return m_manifest[mipmap][element];
}

std::vector<unsigned char> Sprite::LoadSprite() {
	ThrowIfFailed(CoInitialize(
		NULL
	), "COM initialization failed");

	ComPtr<IWICImagingFactory2> pWicFactory{ nullptr };
	ThrowIfFailed(CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pWicFactory)
	), "WIC factory creation failed");

	ComPtr<IWICBitmap> pBmp{ nullptr };
	WICPixelFormatGUID format{ GUID_WICPixelFormat32bppBGR };
	ThrowIfFailed(pWicFactory->CreateBitmap(
		SPRITE_WIDTH, SPRITE_HEGHT,
		format,
		WICBitmapCacheOnDemand,
		&pBmp
	), "Bitmap creation failed");

	ComPtr<IDWriteFactory7> pDwFactory{ nullptr };
	ThrowIfFailed(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(pDwFactory),
		reinterpret_cast<IUnknown**>(pDwFactory.ReleaseAndGetAddressOf())
	), "DirectWrite factory creation failed");

	ComPtr<ID2D1Factory7> pD2Factory{ nullptr };
	ThrowIfFailed(D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		IID_PPV_ARGS(&pD2Factory)
	), "Direct2D factory creation failed");

	ComPtr<ID2D1RenderTarget> pRenderTarget{ nullptr };
	ThrowIfFailed(pD2Factory->CreateWicBitmapRenderTarget(
		pBmp.Get(),
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_IGNORE
			)
		),
		&pRenderTarget
	), "Render target creation failed");

	D2D1_COLOR_F front{ D2D1::ColorF(0.F, 1.F, 0.F, 1.F) };
	D2D1_COLOR_F back{ D2D1::ColorF(0.F, 0.F, 0.F, 1.F) };
	ComPtr<ID2D1SolidColorBrush> pFrontBrush{ nullptr };
	ThrowIfFailed(pRenderTarget->CreateSolidColorBrush(
		front,
		&pFrontBrush
	), "Front brush creation failed");
	ComPtr<ID2D1SolidColorBrush> pBackBrush{ nullptr };
	ThrowIfFailed(pRenderTarget->CreateSolidColorBrush(
		back,
		&pBackBrush
	), "Back brush creation failed");

	ComPtr<ID2D1PathGeometry1> pMark{ nullptr };
	ThrowIfFailed(pD2Factory->CreatePathGeometry(
		&pMark
	), "Mark geometry creation failed");
	{
		ComPtr<ID2D1GeometrySink> pMarkSink{ nullptr };
		ThrowIfFailed(pMark->Open(
			&pMarkSink
		), "Failed to open mark sink");
		pMarkSink->SetFillMode(D2D1_FILL_MODE_WINDING);
		pMarkSink->BeginFigure(D2D1::Point2F(7.F, 16.F), D2D1_FIGURE_BEGIN_FILLED);
		pMarkSink->AddLine(D2D1::Point2F(13.F, 22.F));
		pMarkSink->AddLine(D2D1::Point2F(7.F, 28.F));
		pMarkSink->AddLine(D2D1::Point2F(7.F, 16.F));
		pMarkSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		ThrowIfFailed(pMarkSink->Close(
		), "Failed to close mark sink");
	}
	ComPtr<ID2D1EllipseGeometry> pOuter{ nullptr };
	ThrowIfFailed(pD2Factory->CreateEllipseGeometry(
		out,
		&pOuter
	), "Outer geometry creation failed");
	ComPtr<ID2D1EllipseGeometry> pInner{ nullptr };
	ThrowIfFailed(pD2Factory->CreateEllipseGeometry(
		D2D1::Ellipse(out.point, out.radiusX - stroke, out.radiusY - stroke),
		&pInner
	), "Inner geometry creation failed");

	ComPtr<IDWriteTextFormat> pTextFormat{ nullptr };
	ThrowIfFailed(pDwFactory->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		25.F,
		L"",
		&pTextFormat
	), "Text format creation failed");
	ThrowIfFailed(pTextFormat->SetTextAlignment(
		DWRITE_TEXT_ALIGNMENT_CENTER
	), "Failed to set text alignment");
	ThrowIfFailed(pTextFormat->SetParagraphAlignment(
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER
	), "Failed to set paragraph alignment");
	ComPtr<IDWriteTextLayout> pOutALayout{ nullptr };
	ThrowIfFailed(pDwFactory->CreateTextLayout(
		L"A", 1U,
		pTextFormat.Get(),
		2 * out.radiusX, 2 * out.radiusY,
		&pOutALayout
	), "Output A text layout creation failed");
	ComPtr<IDWriteTextLayout> pOutBLayout{ nullptr };
	ThrowIfFailed(pDwFactory->CreateTextLayout(
		L"B", 1U,
		pTextFormat.Get(),
		2 * out.radiusX, 2 * out.radiusY,
		&pOutBLayout
	), "Output A text layout creation failed");

	auto DrawAct = [&](IDWriteTextLayout* pTextLayout)->void {
		pRenderTarget->FillGeometry(pOuter.Get(), pFrontBrush.Get());
		pRenderTarget->FillGeometry(pMark.Get(), pBackBrush.Get());
		pRenderTarget->DrawTextLayout(D2D1::Point2F(), pTextLayout, pBackBrush.Get());
	};
	auto DrawInact = [&](IDWriteTextLayout* pTextLayout)->void {
		pRenderTarget->FillGeometry(pOuter.Get(), pFrontBrush.Get());
		pRenderTarget->FillGeometry(pInner.Get(), pBackBrush.Get());
		pRenderTarget->FillGeometry(pMark.Get(), pFrontBrush.Get());
		pRenderTarget->DrawTextLayout(D2D1::Point2F(), pTextLayout, pFrontBrush.Get());
	};

	pRenderTarget->BeginDraw();
	pRenderTarget->Clear(back);
	// out_a_act 1x
	DrawAct(pOutALayout.Get());
	// out_b_act 1x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(0.F, 2 * out.radiusY));
	DrawAct(pOutBLayout.Get());
	// out_a_inact 1x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(0.F, 4 * out.radiusY));
	DrawInact(pOutALayout.Get());
	// out_b_inact 1x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(0.F, 6 * out.radiusY));
	DrawInact(pOutBLayout.Get());
	// out_a_act 2x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(2.F, 2.F) * D2D1::Matrix3x2F::Translation(2 * out.radiusX, 0.F));
	DrawAct(pOutALayout.Get());
	// out_b_act 2x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(2.F, 2.F) * D2D1::Matrix3x2F::Translation(6 * out.radiusX, 0.F));
	DrawAct(pOutBLayout.Get());
	// out_a_inact 2x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(2.F, 2.F) * D2D1::Matrix3x2F::Translation(2 * out.radiusX, 4 * out.radiusY));
	DrawInact(pOutALayout.Get());
	// out_b_inact 2x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(2.F, 2.F) * D2D1::Matrix3x2F::Translation(6 * out.radiusX, 4 * out.radiusY));
	DrawInact(pOutBLayout.Get());
	ThrowIfFailed(pRenderTarget->EndDraw(
	), "Sprite creation failed");

	std::vector<unsigned char> buffer(SPRITE_WIDTH * SPRITE_HEGHT * (32 / sizeof(unsigned char)));

	ThrowIfFailed(pBmp->CopyPixels(
		NULL,
		SPRITE_WIDTH * 4,
		buffer.size(), buffer.data()
	), "Bitmap copy failed");

#ifndef NDEBUG
	ComPtr<IWICStream> pStream{ nullptr };
	ThrowIfFailed(pWicFactory->CreateStream(
		&pStream
	), "Stream creation failed");
	ThrowIfFailed(pStream->InitializeFromFilename(
		LR"(sprite.bmp)",
		GENERIC_WRITE
	), "Stream initialization failed");
	ComPtr<IWICBitmapEncoder> pEncoder{ nullptr };
	ThrowIfFailed(pWicFactory->CreateEncoder(
		GUID_ContainerFormatBmp, NULL,
		&pEncoder
	), "Encoder creation failed");
	ThrowIfFailed(pEncoder->Initialize(
		pStream.Get(),
		WICBitmapEncoderNoCache
	), "Encoder initialization failed");
	ComPtr<IWICBitmapFrameEncode> pFrame{ nullptr };
	ThrowIfFailed(pEncoder->CreateNewFrame(
		&pFrame, NULL
	), "Frame creation failed");
	ThrowIfFailed(pFrame->Initialize(
		NULL
	), "Frame initialization failed");
	ThrowIfFailed(pFrame->SetSize(
		SPRITE_WIDTH, SPRITE_HEGHT
	), "Frame size update failed");
	ThrowIfFailed(pFrame->SetPixelFormat(
		&format
	), "Frame pixel format update failed");
	ThrowIfFailed(pFrame->WritePixels(
		SPRITE_HEGHT, SPRITE_WIDTH * 4,
		buffer.size(), buffer.data()
	), "Frame write failed");
#endif

	return buffer;
}