#include <stdexcept>
#include <array>>
#include <vector>
#include <string>
#include <fstream>

#include <wrl/client.h>
#include <wincodec.h>
#include <d2d1_3.h>
#pragma comment(lib, "d2d1")
#include <dwrite_3.h>
#pragma comment(lib, "dwrite")

using Microsoft::WRL::ComPtr;

static constexpr size_t ATLAS_WIDTH{ 230 };
static constexpr size_t ATLAS_HEGHT{ 184 };
static constexpr size_t MIPMAP_COUNT{ 2 };
static constexpr size_t OUTPUT_R{ 23 };
static constexpr size_t STROKE{ 2 };
static constexpr D2D1_ELLIPSE OUTPUT{
	D2D1_POINT_2F{
		OUTPUT_R, OUTPUT_R
	},
	OUTPUT_R, OUTPUT_R
};
static constexpr D2D1_POINT_2F P_MARK_POINT[]{
	D2D1_POINT_2F{ 7.F, 16.F },
	D2D1_POINT_2F{ 13.F, 22.F },
	D2D1_POINT_2F{ 7.F, 28.F }
};
static constexpr D2D1_COLOR_F FRONT{ 1.F, 0.F, 0.F, 1.F };
static constexpr D2D1_COLOR_F BACK{ 0.F, 0.F, 0.F, 1.F };
static constexpr LPCWSTR P_FONT_NAME{ L"Arial" };
static constexpr size_t FONT_SIZE{ 25 };
static constexpr LPCWSTR PP_OUTPUT[]{
	L"A", L"B"
};
enum Image_Id : size_t {
	out_a_act = 0,
	out_b_act = 1,
	out_a_inact = 2,
	out_b_inact = 3
};
static LPCWSTR PP_PXL[]{
	L"out_a_act",
	L"out_b_act",
	L"out_a_inact",
	L"out_b_inact",
};
using MipmapLevel = std::array<WICRect, out_b_inact + 1>;
using MipmapLevels = std::array<MipmapLevel, MIPMAP_COUNT>;
static constexpr MipmapLevels PP_IMAGE{
	MipmapLevel{// 0
		WICRect{// out_a_act
			2 * OUTPUT_R, 0,
			4 * OUTPUT_R, 4 * OUTPUT_R
		},
		WICRect{// out_b_act
			6 * OUTPUT_R, 0,
			4 * OUTPUT_R, 4 * OUTPUT_R
		},
		WICRect{// out_a_inact
			2 * OUTPUT_R, 4 * OUTPUT_R,
			4 * OUTPUT_R, 4 * OUTPUT_R
		},
		WICRect{// out_b_inact_0
			6 * OUTPUT_R, 4 * OUTPUT_R,
			4 * OUTPUT_R, 4 * OUTPUT_R
		}
	},
	MipmapLevel{// 1
		WICRect{// out_a_act
			0, 0,
			2 * OUTPUT_R, 2 * OUTPUT_R
		},
		WICRect{// out_b_act
			0, 2 * OUTPUT_R,
			2 * OUTPUT_R, 2 * OUTPUT_R
		},
		WICRect{// out_a_inact
			0, 4 * OUTPUT_R,
			2 * OUTPUT_R, 2 * OUTPUT_R
		},
		WICRect{// out_b_inact
			0, 6 * OUTPUT_R,
			2 * OUTPUT_R, 2 * OUTPUT_R
		}
	}
};

class com_error : public std::runtime_error {
public:
	inline com_error(HRESULT code_arg, const char* what_arg) noexcept
		: runtime_error{ what_arg }
		, m_errCode{ code_arg } {

	};

	inline HRESULT code() const noexcept {
		return m_errCode;
	};

private:
	const HRESULT m_errCode;
};

inline bool ThrowIfFailed(HRESULT code, const char* what) {
	if (FAILED(code)) {
		throw com_error{ code, what };
	}
	return false;
}

int main() {
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

	ComPtr<IWICBitmap> pBmp{ nullptr };
	WICPixelFormatGUID format{ GUID_WICPixelFormat32bppBGR };
	ThrowIfFailed(pWicFactory->CreateBitmap(
		ATLAS_WIDTH, ATLAS_HEGHT,
		format,
		WICBitmapCacheOnDemand,
		&pBmp
	), "Bitmap creation failed");

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

	ComPtr<ID2D1SolidColorBrush> pFrontBrush{ nullptr };
	ThrowIfFailed(pRenderTarget->CreateSolidColorBrush(
		FRONT,
		&pFrontBrush
	), "Front brush creation failed");
	ComPtr<ID2D1SolidColorBrush> pBackBrush{ nullptr };
	ThrowIfFailed(pRenderTarget->CreateSolidColorBrush(
		BACK,
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
		pMarkSink->BeginFigure(P_MARK_POINT[0], D2D1_FIGURE_BEGIN_FILLED);
		for (size_t i{ 1 }; i < std::size(P_MARK_POINT); ++i) {
			pMarkSink->AddLine(P_MARK_POINT[i]);
		}
		pMarkSink->AddLine(P_MARK_POINT[0]);
		pMarkSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		ThrowIfFailed(pMarkSink->Close(
		), "Failed to close mark sink");
	}
	ComPtr<ID2D1EllipseGeometry> pOuter{ nullptr };
	ThrowIfFailed(pD2Factory->CreateEllipseGeometry(
		OUTPUT,
		&pOuter
	), "Outer geometry creation failed");
	ComPtr<ID2D1EllipseGeometry> pInner{ nullptr };
	ThrowIfFailed(pD2Factory->CreateEllipseGeometry(
		D2D1::Ellipse(OUTPUT.point, OUTPUT.radiusX - STROKE, OUTPUT.radiusY - STROKE),
		&pInner
	), "Inner geometry creation failed");

	ComPtr<IDWriteTextFormat> pTextFormat{ nullptr };
	ThrowIfFailed(pDwFactory->CreateTextFormat(
		P_FONT_NAME,
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		FONT_SIZE,
		L"",
		&pTextFormat
	), "Text format creation failed");
	ThrowIfFailed(pTextFormat->SetTextAlignment(
		DWRITE_TEXT_ALIGNMENT_CENTER
	), "Failed to set text alignment");
	ThrowIfFailed(pTextFormat->SetParagraphAlignment(
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER
	), "Failed to set paragraph alignment");

	std::array<ComPtr<IDWriteTextLayout>, std::size(PP_OUTPUT)> ppOutLayout{};
	for (size_t out{ 0 }; out < std::size(PP_OUTPUT); ++out) {
		ThrowIfFailed(pDwFactory->CreateTextLayout(
			PP_OUTPUT[out], 1U,
			pTextFormat.Get(),
			2 * OUTPUT.radiusX, 2 * OUTPUT.radiusY,
			&ppOutLayout[out]
		), "Output A text layout creation failed");
	}

	auto DrawOutAct = [&](IDWriteTextLayout* pTextLayout)->void {
		pRenderTarget->FillGeometry(pOuter.Get(), pFrontBrush.Get());
		pRenderTarget->FillGeometry(pMark.Get(), pBackBrush.Get());
		pRenderTarget->DrawTextLayout(D2D1::Point2F(), pTextLayout, pBackBrush.Get());
	};
	auto DrawOutInact = [&](IDWriteTextLayout* pTextLayout)->void {
		pRenderTarget->FillGeometry(pOuter.Get(), pFrontBrush.Get());
		pRenderTarget->FillGeometry(pInner.Get(), pBackBrush.Get());
		pRenderTarget->FillGeometry(pMark.Get(), pFrontBrush.Get());
		pRenderTarget->DrawTextLayout(D2D1::Point2F(), pTextLayout, pFrontBrush.Get());
	};

	pRenderTarget->BeginDraw();
	pRenderTarget->Clear(BACK);
	for (size_t mipmap{ 0 }; mipmap < MIPMAP_COUNT; ++mipmap) {
		FLOAT factor{ powf(2.F, MIPMAP_COUNT - 1 - mipmap) };
		D2D1_MATRIX_3X2_F scale{ D2D1::Matrix3x2F::Scale(factor, factor) };
		for (size_t id{ out_a_act }; id <= out_b_act; ++id) {
			const WICRect& rect{ PP_IMAGE[mipmap][id] };
			IDWriteTextLayout* layout{ ppOutLayout[id % std::size(PP_OUTPUT)].Get() };

			pRenderTarget->SetTransform(scale * D2D1::Matrix3x2F::Translation(rect.X, rect.Y));
			DrawOutAct(layout);
		}
		for (size_t id{ out_a_inact }; id <= out_b_inact; ++id) {
			const WICRect& rect{ PP_IMAGE[mipmap][id] };
			IDWriteTextLayout* layout{ ppOutLayout[id % std::size(PP_OUTPUT)].Get() };

			pRenderTarget->SetTransform(scale * D2D1::Matrix3x2F::Translation(rect.X, rect.Y));
			DrawOutInact(layout);
		}
	}
	ThrowIfFailed(pRenderTarget->EndDraw(
	), "Sprite creation failed");

	ComPtr<IWICStream> pStream{ nullptr };
	ThrowIfFailed(pWicFactory->CreateStream(
		&pStream
	), "Stream creation failed");
	ThrowIfFailed(pStream->InitializeFromFilename(
		LR"(atlas.bmp)",
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
		ATLAS_WIDTH, ATLAS_HEGHT
	), "Frame size update failed");
	ThrowIfFailed(pFrame->SetPixelFormat(
		&format
	), "Frame pixel format update failed");
	ThrowIfFailed(pFrame->WriteSource(
		pBmp.Get(), NULL
	), "Frame write failed");

	for (size_t mipmap{ 0 }; mipmap < MIPMAP_COUNT; ++mipmap) {
		for (size_t id{ out_a_act }; id <= out_b_inact; ++id) {
			const WICRect& rect{ PP_IMAGE[mipmap][id] };
			const INT stride{ rect.Width * 4 };
			const INT size{ rect.Height * stride };

			std::vector<BYTE> buffer(size);
			ThrowIfFailed(pBmp->CopyPixels(
				&rect,
				stride, size,
				&buffer[0]
			), "Bitmap copy failed");

			std::wstring pxlName{ PP_PXL[id] };
			pxlName.append(L"_")
				.append(std::to_wstring(mipmap))
				.append(L".pxl");
			std::ofstream pxl{ pxlName };

			size_t count{ 0 };
			for (const BYTE byte : buffer) {
				if (count == 2) {
					pxl << byte;
				}
				if (++count == 4) {
					count = 0;
				};
			}
		}
	}
};