#include "drawingengine.h"

#include "../wrappers.h"
#include "../../messages.h"

using namespace Voicemeeter::DeskBand::Windows::Presentation;

DrawingEngine::Context::Context()
	: m_pCtxDevD2d1{ NULL }
	, m_pSwChDxgi{ NULL }
	, m_pBmpD2d1{ NULL } {

	}

void DrawingEngine::Context::BeginDraw() {
	m_pCtxDevD2d1->BeginDraw();
}
void DrawingEngine::Context::EndDraw() {
	ThrowIfFailed(m_pCtxDevD2d1->EndDraw(

	), "Drawing failure");
	DXGI_PRESENT_PARAMETERS params{
		0, NULL,
		NULL, NULL
	};
	ThrowIfFailed(m_pSwChDxgi->Present1(
		1, 0,
		&params
	), "Presentation failure");
}
void DrawingEngine::Context::Resize(UINT w, UINT h) {
	m_pSwChDxgi->ResizeBuffers1(
		0,
		w, h,
		DXGI_FORMAT_UNKNOWN,
		0,
		NULL, NULL
	);
}

DrawingEngine::DrawingEngine(
	const Style& rStyle
) : m_rStyle{ rStyle }
  , m_pDevD3d11{ NULL }
  , m_pDevDxgi{ NULL }
  , m_pAdDxgi{ NULL }
  , m_pFctDxgi{ NULL }
  , m_pFctD2d1{ NULL }
  , m_pDevD2d1{ NULL }
  , m_pFctDw{ NULL } {
		D3D_FEATURE_LEVEL featureLevels[] {
			D3D_FEATURE_LEVEL_12_1
		};
		CComPtr<ID3D11Device> pDevD3d11{ NULL };
		ThrowIfFailed(D3D11CreateDevice(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			D3D11_CREATE_DEVICE_BGRA_SUPPORT,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			&pDevD3d11,
			NULL,
			NULL
		), "D3D11 device creation failed");
		ThrowIfFailed(pDevD3d11->QueryInterface(
			&m_pDevD3d11
		), "D3D11 5 device creation failed");

		ThrowIfFailed(m_pDevD3d11->QueryInterface(
			&m_pDevDxgi
		), "DXGI device creation failed");
		CComPtr<IDXGIAdapter> pAdDxgi{ NULL };
		ThrowIfFailed(m_pDevDxgi->GetAdapter(
			&pAdDxgi
		), "DXGI retrieve adapter failed");
		ThrowIfFailed(pAdDxgi->QueryInterface(
			&m_pAdDxgi
		), "DXGI retrieve adapter failed");
		ThrowIfFailed(m_pAdDxgi->GetParent(
			IID_PPV_ARGS(&m_pFctDxgi)
		), "DXGI retrieve factory failed");

		ThrowIfFailed(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&m_pFctD2d1
		), "D2D1 factory creation failed");

		ThrowIfFailed(m_pFctD2d1->CreateDevice(
			m_pDevDxgi,
			&m_pDevD2d1
		), "D2D1 device creation failed");

		ThrowIfFailed(DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory7),
			reinterpret_cast<IUnknown**>(&m_pFctDw)
		), "DirectWrite factory creation failed");
	}

DrawingEngine::Context DrawingEngine::Initialize(
	HWND hWnd
) {
	Context ctx{};

	ThrowIfFailed(m_pDevD2d1->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&ctx.m_pCtxDevD2d1
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
	CComPtr<IDXGISwapChain1> pSwChDxgi{ NULL };
	ThrowIfFailed(m_pFctDxgi->CreateSwapChainForHwnd(
		m_pDevD3d11,
		hWnd,
		&desc,
		NULL,
		NULL,
		&pSwChDxgi
	), "Swap chain creation failed");
	ThrowIfFailed(pSwChDxgi->QueryInterface(
		&ctx.m_pSwChDxgi
	), "Swap chain creation failed");
	ThrowIfFailed(ctx.m_pSwChDxgi->SetMaximumFrameLatency(
		1
	), "Failed to update maximum frame latency");

	CComPtr<IDXGISurface2> pSfDxgi{ NULL };
	ThrowIfFailed(ctx.m_pSwChDxgi->GetBuffer(
		0,
		IID_PPV_ARGS(&pSfDxgi)
	), "Failed to get surface");

	FLOAT dpiX{ 0.F };
	FLOAT dpiY{ 0.F };
	ctx.m_pCtxDevD2d1->GetDpi(&dpiX, &dpiY);
	ThrowIfFailed(ctx.m_pCtxDevD2d1->CreateBitmapFromDxgiSurface(
		pSfDxgi,
		D2D1_BITMAP_PROPERTIES1{
			D2D1_PIXEL_FORMAT{
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_STRAIGHT
			},
			dpiX, dpiY,
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			NULL
		},
		&ctx.m_pBmpD2d1
	), "Bitmap creation failed");
	ctx.m_pCtxDevD2d1->SetTarget(ctx.m_pBmpD2d1);

	return ctx;
}