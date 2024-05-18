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
	WICPixelFormatGUID format{ GUID_WICPixelFormat32bppPBGRA }; // GUID_WICPixelFormat8bppAlpha
	ThrowIfFailed(pWicFactory->CreateBitmap(
		SPRITE_WIDTH, SPRITE_HEGHT,
		format,
		WICBitmapCacheOnDemand,
		&pBmp
	), "Bitmap creation failed");

	ComPtr<ID2D1Factory7> pD2Factory{ nullptr };
	ThrowIfFailed(D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		IID_PPV_ARGS(&pD2Factory)
	), "Direct2D factory creation failed");

	ComPtr<IDWriteFactory7> pDwFactory{ nullptr };
	ThrowIfFailed(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(pDwFactory),
		reinterpret_cast<IUnknown**>(pDwFactory.ReleaseAndGetAddressOf())
	), "DirectWrite factory creation failed");

	ComPtr<ID2D1RenderTarget> pRenderTarget{ nullptr };
	ThrowIfFailed(pD2Factory->CreateWicBitmapRenderTarget(
		pBmp.Get(),
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_PREMULTIPLIED
			)
		),
		&pRenderTarget
	), "Render target creation failed");

	D2D1_ELLIPSE out_act{ D2D1::Ellipse(D2D1::Point2F(46.F, 46.F), 46.F, 46.F) };
	FLOAT stroke{ 4.F };
	D2D1_ELLIPSE out_inact{ D2D1::Ellipse(out_act.point, out_act.radiusX - stroke / 2, out_act.radiusY - stroke / 2) };
	D2D1_COLOR_F front{ D2D1::ColorF(1.F, 1.F, 1.F, 1.F) };
	D2D1_COLOR_F back{ D2D1::ColorF(0.F, 0.F, 0.F, 0.F) };

	ComPtr<ID2D1SolidColorBrush> pFrontBrush{ nullptr };
	ThrowIfFailed(pRenderTarget->CreateSolidColorBrush(
		front,
		&pFrontBrush
	), "Front brush creation failed");
	ComPtr<ID2D1SolidColorBrush> pBackBrush{ nullptr };
	ThrowIfFailed(pRenderTarget->CreateSolidColorBrush(
		front,
		&pBackBrush
	), "Back brush creation failed");

	pRenderTarget->BeginDraw();
	pRenderTarget->Clear(back);
	// out_a_act 2x
	pRenderTarget->FillEllipse(out_act, pFrontBrush.Get());
	// out_b_act 2x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(2 * out_act.radiusX, 0.F));
	pRenderTarget->FillEllipse(out_act, pFrontBrush.Get());
	// out_a_inact 2x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(0.F, 2 * out_act.radiusY));
	pRenderTarget->DrawEllipse(out_inact, pFrontBrush.Get(), stroke);
	// out_b_inact 2x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(2 * out_act.radiusX, 2 * out_act.radiusY));
	pRenderTarget->DrawEllipse(out_inact, pFrontBrush.Get(), stroke);
	// out_a_act 1x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(D2D1::SizeF(0.5F, 0.5F))
		* D2D1::Matrix3x2F::Translation(4 * out_act.radiusX, 0.F));
	pRenderTarget->FillEllipse(out_act, pFrontBrush.Get());
	// out_b_act 1x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(D2D1::SizeF(0.5F, 0.5F))
		* D2D1::Matrix3x2F::Translation(4 * out_act.radiusX, out_act.radiusY));
	pRenderTarget->FillEllipse(out_act, pFrontBrush.Get());
	// out_a_inact 1x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(D2D1::SizeF(0.5F, 0.5F))
		* D2D1::Matrix3x2F::Translation(4 * out_act.radiusX, 2 * out_act.radiusY));
	pRenderTarget->DrawEllipse(out_inact, pFrontBrush.Get(), stroke);
	// out_b_inact 1x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(D2D1::SizeF(0.5F, 0.5F))
		* D2D1::Matrix3x2F::Translation(4 * out_act.radiusX, 3 * out_act.radiusY));
	pRenderTarget->DrawEllipse(out_inact, pFrontBrush.Get(), stroke);
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