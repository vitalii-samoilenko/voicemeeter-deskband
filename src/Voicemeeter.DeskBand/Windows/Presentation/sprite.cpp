#include "sprite.h"

using namespace Voicemeeter::DeskBand::Windows::Presentation;

static constexpr size_t SPRITE_WIDTH{ 230 };
static constexpr size_t SPRITE_HEGHT{ 184 };
static constexpr size_t OUT_R{ 23 };
static constexpr size_t P_SIZE{ 32 / sizeof(Sprite::BYTE) };

#ifndef NDEBUG
#include <wrl/client.h>
#include <wincodec.h>
#include <d2d1_3.h>
#pragma comment(lib, "d2d1")
#include <dwrite_3.h>
#pragma comment(lib, "dwrite")

#include "../wrappers.h"

using Microsoft::WRL::ComPtr;

using MipmapLevel = std::array<WICRect, 4>;
using MipmapLevels = std::array<MipmapLevel, Sprite::MIPMAP_COUNT>;

inline static MipmapLevels GetLayout(const Sprite::manifest_type& manifest) {
	return MipmapLevels{
		MipmapLevel{
			WICRect{
				2 * OUT_R, 0,
				static_cast<INT>(manifest[0][Sprite::out_a_act].Width),
				static_cast<INT>(manifest[0][Sprite::out_a_act].Height)
			},
			WICRect{
				6 * OUT_R, 0,
				static_cast<INT>(manifest[0][Sprite::out_b_act].Width),
				static_cast<INT>(manifest[0][Sprite::out_b_act].Height)
			},
			WICRect{
				2 * OUT_R, 4 * OUT_R,
				static_cast<INT>(manifest[0][Sprite::out_a_inact].Width),
				static_cast<INT>(manifest[0][Sprite::out_a_inact].Height)
			},
			WICRect{
				6 * OUT_R, 4 * OUT_R,
				static_cast<INT>(manifest[0][Sprite::out_b_inact].Width),
				static_cast<INT>(manifest[0][Sprite::out_b_inact].Height)
			}
		},
		MipmapLevel{
			WICRect{
				0, 0,
				static_cast<INT>(manifest[1][Sprite::out_a_act].Width),
				static_cast<INT>(manifest[1][Sprite::out_a_act].Height)
			},
			WICRect{
				0, 2 * OUT_R,
				static_cast<INT>(manifest[1][Sprite::out_b_act].Width),
				static_cast<INT>(manifest[1][Sprite::out_b_act].Height)
			},
			WICRect{
				0, 4 * OUT_R,
				static_cast<INT>(manifest[1][Sprite::out_a_inact].Width),
				static_cast<INT>(manifest[1][Sprite::out_a_inact].Height)
			},
			WICRect{
				0, 6 * OUT_R,
				static_cast<INT>(manifest[1][Sprite::out_b_inact].Width),
				static_cast<INT>(manifest[1][Sprite::out_b_inact].Height)
			}
		}
	};
}

static void DrawSprite(Sprite::buffer_type& buffer, const Sprite::manifest_type& manifest) {
	MipmapLevels ppRect{ GetLayout(manifest) };

	D2D1_ELLIPSE out{ D2D1::Ellipse(D2D1::Point2F(OUT_R, OUT_R), OUT_R, OUT_R) };
	FLOAT stroke{ 2.F };

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
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(ppRect[1][Sprite::out_a_act].X, ppRect[1][Sprite::out_a_act].Y));
	DrawAct(pOutALayout.Get());
	// out_b_act 1x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(ppRect[1][Sprite::out_b_act].X, ppRect[1][Sprite::out_b_act].Y));
	DrawAct(pOutBLayout.Get());
	// out_a_inact 1x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(ppRect[1][Sprite::out_a_inact].X, ppRect[1][Sprite::out_a_inact].Y));
	DrawInact(pOutALayout.Get());
	// out_b_inact 1x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(ppRect[1][Sprite::out_b_inact].X, ppRect[1][Sprite::out_b_inact].Y));
	DrawInact(pOutBLayout.Get());
	// out_a_act 2x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(2.F, 2.F) * D2D1::Matrix3x2F::Translation(ppRect[0][Sprite::out_a_act].X, ppRect[0][Sprite::out_a_act].Y));
	DrawAct(pOutALayout.Get());
	// out_b_act 2x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(2.F, 2.F) * D2D1::Matrix3x2F::Translation(ppRect[0][Sprite::out_b_act].X, ppRect[0][Sprite::out_b_act].Y));
	DrawAct(pOutBLayout.Get());
	// out_a_inact 2x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(2.F, 2.F) * D2D1::Matrix3x2F::Translation(ppRect[0][Sprite::out_a_inact].X, ppRect[0][Sprite::out_a_inact].Y));
	DrawInact(pOutALayout.Get());
	// out_b_inact 2x
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(2.F, 2.F) * D2D1::Matrix3x2F::Translation(ppRect[0][Sprite::out_b_inact].X, ppRect[0][Sprite::out_b_inact].Y));
	DrawInact(pOutBLayout.Get());
	ThrowIfFailed(pRenderTarget->EndDraw(
	), "Sprite creation failed");

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
	ThrowIfFailed(pFrame->WriteSource(
		pBmp.Get(), NULL
	), "Frame write failed");

	for (size_t mipmap{ 0 }; mipmap < Sprite::MIPMAP_COUNT; ++mipmap) {
		for (size_t element{ Sprite::out_a_act }; element <= Sprite::out_b_inact; ++element) {
			ThrowIfFailed(pBmp->CopyPixels(
				&ppRect[mipmap][element],
				manifest[mipmap][element].RowPitch,
				manifest[mipmap][element].SlicePitch,
				manifest[mipmap][element].pData
			), "Bitmap copy failed");
		}
	}
}

static void LoadSprite(Sprite::buffer_type& buffer, const Sprite::manifest_type& manifest) {
	MipmapLevels ppRect{ GetLayout(manifest) };

	WICPixelFormatGUID format{ GUID_WICPixelFormat32bppBGR };

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

	ComPtr<IWICBitmapDecoder> pDecoder{ nullptr };
	ThrowIfFailed(pWicFactory->CreateDecoderFromFilename(
		LR"(sprite.bmp)", NULL, GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	), "Decoder creation failed");
	ComPtr<IWICBitmapFrameDecode> pFrame{ nullptr };
	ThrowIfFailed(pDecoder->GetFrame(
		0U, &pFrame
	), "Frame creation failed");

	for (size_t mipmap{ 0 }; mipmap < Sprite::MIPMAP_COUNT; ++mipmap) {
		for (size_t element{ Sprite::out_a_act }; element <= Sprite::out_b_inact; ++element) {
			ThrowIfFailed(pFrame->CopyPixels(
				&ppRect[mipmap][element],
				manifest[mipmap][element].RowPitch,
				manifest[mipmap][element].SlicePitch,
				manifest[mipmap][element].pData
			), "Bitmap copy failed");
		}
	}
}
#endif

Sprite::Region Sprite::get_Region(Sprite_element element, size_t mipmap) const {
	return m_manifest[mipmap][element];
}

Sprite::Sprite()
	: m_buffer(SPRITE_WIDTH * SPRITE_HEGHT * P_SIZE)
	, m_manifest{} {
	for (size_t offset{ 0 }, mipmap{ 0 }, d{ OUT_R * 4 }; mipmap < MIPMAP_COUNT; ++mipmap, d /= 2) {
		for (size_t element{ out_a_act }, rowPitch{ d * P_SIZE }, slicePitch{ rowPitch * d }; element <= out_b_inact; ++element) {
			m_manifest[mipmap].push_back(Region{
				d, d,
				&m_buffer[offset],
				rowPitch,
				slicePitch
			});
			offset += slicePitch;
		}
	}
#ifndef NDEBUG
	//DrawSprite(m_buffer, m_manifest);
	LoadSprite(m_buffer, m_manifest);
#endif
}