#include "drawingengine.h"

#include "../wrappers.h"
#include "../../messages.h"

using namespace Voicemeeter::DeskBand::Windows::Presentation;

static const FLOAT BTN_ROUND_R{ 23.F };
static const FLOAT SHAPE_MAX_ZOOM_FACTOR{ 2.F };

void DrawingEngine::Context::Text::Draw(const DrawingEngine::Context::Brush& brush) const {
	m_ctx.m_pCtxDevD2->DrawTextLayout(
		D2D1::Point2F(),
		m_pLayoutDw,
		brush.m_pBrushD2
	);
}
void DrawingEngine::Context::Glyph::Draw(const DrawingEngine::Context::Brush& brush) const {
	m_ctx.m_pCtxDevD2->DrawGeometryRealization(
		m_pRealizationD2,
		brush.m_pBrushD2
	);
}

void DrawingEngine::Context::BeginDraw() const {
	m_pCtxDevD2->BeginDraw();
}
void DrawingEngine::Context::EndDraw() const {
	ThrowIfFailed(m_pCtxDevD2->EndDraw(

	), "Drawing failure");
	DXGI_PRESENT_PARAMETERS params{
		0, NULL,
		NULL, NULL
	};
	ThrowIfFailed(m_pSwChDx->Present1(
		1, 0,
		&params
	), "Presentation failure");
}
void DrawingEngine::Context::Resize(UINT w, UINT h) const {
	m_pSwChDx->ResizeBuffers1(
		0,
		w, h,
		DXGI_FORMAT_UNKNOWN,
		0,
		NULL, NULL
	);
}

DrawingEngine::Context::Text DrawingEngine::Manifest::Font::Bind(DrawingEngine::Context& ctx,
	const LPCWSTR text, FLOAT w, FLOAT h
) const {
	DrawingEngine::Context::Text t{ ctx };
	ThrowIfFailed(ctx.m_fctDw.CreateTextLayout(
		text, static_cast<UINT32>(wcslen(text)),
		m_pFmtDw,
		w, h,
		reinterpret_cast<IDWriteTextLayout**>(& t.m_pLayoutDw)
	), "Text creation failed");
	return t;
}
DrawingEngine::Context::Brush DrawingEngine::Manifest::Color::Bind(DrawingEngine::Context& ctx) const {
	DrawingEngine::Context::Brush b{ ctx };
	ThrowIfFailed(ctx.m_pCtxDevD2->CreateSolidColorBrush(
		m_colorD2,
		&b.m_pBrushD2
	), "Brush creation failed");
	return b;
}
DrawingEngine::Context::Glyph DrawingEngine::Manifest::Outline::Bind(DrawingEngine::Context& ctx) const {
	FLOAT dpiX{ 0.F };
	FLOAT dpiY{ 0.F };
	ctx.m_pCtxDevD2->GetDpi(&dpiX, &dpiY);
	DrawingEngine::Context::Glyph g{ ctx };
	ThrowIfFailed(ctx.m_pCtxDevD2->CreateStrokedGeometryRealization(
		m_pGmtD2,
		D2D1::ComputeFlatteningTolerance(
			D2D1::Matrix3x2F::Identity(),
			dpiX, dpiY,
			SHAPE_MAX_ZOOM_FACTOR
		),
		m_w, NULL,
		&g.m_pRealizationD2
	), "Glyph creation failed");
	return g;
}
DrawingEngine::Context::Glyph DrawingEngine::Manifest::Interior::Bind(DrawingEngine::Context& ctx) const {
	FLOAT dpiX{ 0.F };
	FLOAT dpiY{ 0.F };
	ctx.m_pCtxDevD2->GetDpi(&dpiX, &dpiY);
	DrawingEngine::Context::Glyph g{ ctx };
	ThrowIfFailed(ctx.m_pCtxDevD2->CreateFilledGeometryRealization(
		m_pGmtD2,
		D2D1::ComputeFlatteningTolerance(
			D2D1::Matrix3x2F::Identity(),
			dpiX, dpiY,
			SHAPE_MAX_ZOOM_FACTOR
		),
		&g.m_pRealizationD2
	), "Glyph creation failed");
	return g;
}

DrawingEngine::DrawingEngine(const Style& style)
	: m_pDevD3{ NULL }
	, m_pDevDx{ NULL }
	, m_pAdDx{ NULL }
	, m_pFctDx{ NULL }
	, m_pFctD2{ NULL }
	, m_pDevD2{ NULL }
	, m_pFctDw{ NULL }
	, m_manifest{} {
	D3D_FEATURE_LEVEL featureLevels[]{
		D3D_FEATURE_LEVEL_12_1
	};
	CComPtr<ID3D11Device> pDevD3d{ NULL };
	ThrowIfFailed(D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&pDevD3d,
		NULL,
		NULL
	), "D3D11 device creation failed");
	ThrowIfFailed(pDevD3d->QueryInterface(
		&m_pDevD3
	), "D3D11 5 device creation failed");

	ThrowIfFailed(m_pDevD3->QueryInterface(
		&m_pDevDx
	), "DXGI device creation failed");
	CComPtr<IDXGIAdapter> pAdDx{ NULL };
	ThrowIfFailed(m_pDevDx->GetAdapter(
		&pAdDx
	), "DXGI retrieve adapter failed");
	ThrowIfFailed(pAdDx->QueryInterface(
		&m_pAdDx
	), "DXGI retrieve adapter failed");
	ThrowIfFailed(m_pAdDx->GetParent(
		IID_PPV_ARGS(&m_pFctDx)
	), "DXGI retrieve factory failed");

	ThrowIfFailed(D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&m_pFctD2
	), "D2D1 factory creation failed");

	ThrowIfFailed(m_pFctD2->CreateDevice(
		m_pDevDx,
		&m_pDevD2
	), "D2D1 device creation failed");

	ThrowIfFailed(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory7),
		reinterpret_cast<IUnknown**>(&m_pFctDw)
	), "DirectWrite factory creation failed");

	ThrowIfFailed(m_pFctDw->CreateTextFormat(
		style.lpszMainFontFamilyName, nullptr,
		NULL, 0U,
		12.F,
		L"",
		&m_manifest.m_fMain.m_pFmtDw
	), "Main font creation failed");
	m_manifest.m_cFront.m_colorD2 = style.Front;
	m_manifest.m_cActive.m_colorD2 = style.Active;
	m_manifest.m_oBtnRound.m_w = 2.F;
	ThrowIfFailed(m_pFctD2->CreateEllipseGeometry(
		D2D1::Ellipse(
			D2D1::Point2F(BTN_ROUND_R, BTN_ROUND_R),
			BTN_ROUND_R, BTN_ROUND_R
		),
		reinterpret_cast<ID2D1EllipseGeometry**>(&m_manifest.m_oBtnRound.m_pGmtD2)
	), "Round button outline creation failed");
	ThrowIfFailed(m_pFctD2->CreateEllipseGeometry(
		D2D1::Ellipse(
			D2D1::Point2F(BTN_ROUND_R, BTN_ROUND_R),
			BTN_ROUND_R, BTN_ROUND_R
		),
		reinterpret_cast<ID2D1EllipseGeometry**>(&m_manifest.m_iBtnRound.m_pGmtD2)
	), "Round button interior creation failed");
}

DrawingEngine::Context DrawingEngine::Initialize(
	HWND hWnd
) const {
	Context ctx{*m_pFctDw};

	ThrowIfFailed(m_pDevD2->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&ctx.m_pCtxDevD2
	), "D2D1 device context creation failed");

	DXGI_SWAP_CHAIN_DESC1 desc{
		0U, 0U,
		DXGI_FORMAT_B8G8R8A8_UNORM,
		FALSE,
		DXGI_SAMPLE_DESC{
			1U, 0U
		},
		DXGI_USAGE_RENDER_TARGET_OUTPUT, 2U,
		DXGI_SCALING_NONE,
		DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
		DXGI_ALPHA_MODE_UNSPECIFIED,
		0U
	};
	CComPtr<IDXGISwapChain1> pSwChDx{ NULL };
	ThrowIfFailed(m_pFctDx->CreateSwapChainForHwnd(
		m_pDevD3,
		hWnd,
		&desc,
		NULL,
		NULL,
		&pSwChDx
	), "Swap chain creation failed");
	ThrowIfFailed(pSwChDx->QueryInterface(
		&ctx.m_pSwChDx
	), "Swap chain creation failed");
	ThrowIfFailed(ctx.m_pSwChDx->SetMaximumFrameLatency(
		1
	), "Failed to update maximum frame latency");

	CComPtr<IDXGISurface2> pSfDx{ NULL };
	ThrowIfFailed(ctx.m_pSwChDx->GetBuffer(
		0,
		IID_PPV_ARGS(&pSfDx)
	), "Failed to get surface");

	FLOAT dpiX{ 0.F };
	FLOAT dpiY{ 0.F };
	ctx.m_pCtxDevD2->GetDpi(&dpiX, &dpiY);
	ThrowIfFailed(ctx.m_pCtxDevD2->CreateBitmapFromDxgiSurface(
		pSfDx,
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_STRAIGHT
			),
			dpiX, dpiY
		),
		&ctx.m_pBmpD2
	), "Bitmap creation failed");
	ctx.m_pCtxDevD2->SetTarget(ctx.m_pBmpD2);

	return ctx;
}