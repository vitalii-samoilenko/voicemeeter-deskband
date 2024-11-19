#include <array>
#include <cmath>
#include <tuple>
#include <utility>

#include <d3d11_4.h>

#include "Windows/Wrappers.h"

#include "Canvas.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dcomp")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "uxtheme")

using namespace ::Voicemeeter::UI::D2D::Graphics;

Canvas::Canvas(
	HWND hWnd,
	const Theme& theme,
	::Environment::ITimer& timer
) : m_point{ 0., 0. }
  , m_vertex{ 8., 8. }
  , m_pDwFactory{ nullptr }
  , m_pD2dFactory{ nullptr }
  , m_pD2dDeviceContext{ nullptr }
  , m_pDxgiSwapChain{ nullptr }
  , m_pCompositionTarget{ nullptr }
  , m_pPalette{ nullptr }
  , m_pQueue{ nullptr } {
	::Windows::ThrowIfFailed(CoInitialize(
		NULL
	), "COM initialization failed");

	::Windows::ThrowIfFailed(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_pDwFactory),
		reinterpret_cast<IUnknown**>(m_pDwFactory.ReleaseAndGetAddressOf())
	), "DirectWrite factory creation failed");

	::Windows::ThrowIfFailed(D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
#ifndef NDEBUG
		D2D1_FACTORY_OPTIONS{
			D2D1_DEBUG_LEVEL_INFORMATION
		},
#endif // !NDEBUG
		m_pD2dFactory.ReleaseAndGetAddressOf()
	), "Direct2D factory creation failed");

	D3D_FEATURE_LEVEL pFeatureLevel[] {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};
	::Microsoft::WRL::ComPtr<ID3D11Device> pD3dDevice{ nullptr };
	::Microsoft::WRL::ComPtr<ID3D11DeviceContext> pD3dContext{ nullptr };
	::Windows::ThrowIfFailed(D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE, NULL,
		D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT
#ifndef NDEBUG
		| D3D11_CREATE_DEVICE_DEBUG,
#else
		,
#endif // NDEBUG
		pFeatureLevel, static_cast<UINT>(::std::size(pFeatureLevel)), D3D11_SDK_VERSION,
		&pD3dDevice, nullptr, &pD3dContext
	), "Direct3D device creation failed");

	::Microsoft::WRL::ComPtr<IDXGIDevice1> pDxgiDevice{ nullptr };
	::Windows::ThrowIfFailed(pD3dDevice.As(
		&pDxgiDevice
	), "Could not get DXGI device");

	::Microsoft::WRL::ComPtr<ID2D1Device5> pD2dDevice{ nullptr };
	::Windows::ThrowIfFailed(m_pD2dFactory->CreateDevice(
		pDxgiDevice.Get(),
		&pD2dDevice
	), "Direct2D device creation failed");

	::Windows::ThrowIfFailed(pD2dDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&m_pD2dDeviceContext
	), "Direct2D device context creation failed");

	::Microsoft::WRL::ComPtr<IDXGIAdapter> pDxgiAdapter{ nullptr };
	::Windows::ThrowIfFailed(pDxgiDevice->GetAdapter(
		&pDxgiAdapter
	), "Could not get DXGI adapter");

	::Microsoft::WRL::ComPtr<IDXGIFactory2> pDxgiFactory{ nullptr };
	::Windows::ThrowIfFailed(pDxgiAdapter->GetParent(
		IID_PPV_ARGS(&pDxgiFactory)
	), "Could not get DXGI factory");

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
		static_cast<UINT>(m_vertex[0]),
		static_cast<UINT>(m_vertex[1]),
		DXGI_FORMAT_B8G8R8A8_UNORM,
		FALSE,
		DXGI_SAMPLE_DESC{
			1U, 0U
		},
		DXGI_USAGE_RENDER_TARGET_OUTPUT, 2U,
		DXGI_SCALING_STRETCH,
		DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
		DXGI_ALPHA_MODE_PREMULTIPLIED,
		0U
	};
	::Windows::ThrowIfFailed(pDxgiFactory->CreateSwapChainForComposition(
		pD3dDevice.Get(),
		&swapChainDesc,
		nullptr,
		&m_pDxgiSwapChain
	), "DXGI swap chain creation failed");

	::Windows::ThrowIfFailed(pDxgiDevice->SetMaximumFrameLatency(
		1
	), "Could not set maximum frame latency");

	::Microsoft::WRL::ComPtr<IDCompositionDevice> pCompositionDevice{ nullptr };
	::Windows::ThrowIfFailed(DCompositionCreateDevice(
		pDxgiDevice.Get(),
		IID_PPV_ARGS(&pCompositionDevice)
	), "Composition device creation failed");

	::Windows::ThrowIfFailed(pCompositionDevice->CreateTargetForHwnd(
		hWnd, TRUE,
		&m_pCompositionTarget
	), "Composition target creation failed");

	::Microsoft::WRL::ComPtr<IDCompositionVisual> pCompositionVisual{ nullptr };
	::Windows::ThrowIfFailed(pCompositionDevice->CreateVisual(
		&pCompositionVisual
	), "Composition visual creation failed");

	::Windows::ThrowIfFailed(pCompositionVisual->SetContent(
		m_pDxgiSwapChain.Get()
	), "Could not set swap chain content");

	::Windows::ThrowIfFailed(m_pCompositionTarget->SetRoot(
		pCompositionVisual.Get()
	), "Could not set composition target root");

	::Windows::ThrowIfFailed(pCompositionDevice->Commit(
	), "Could not commit composition device");

	m_pPalette.reset(new Palette{ theme, *this });
	m_pQueue.reset(new Queue{ timer, *this });

	ResetTarget();
}

const ::std::valarray<double>& Canvas::get_Position() const {
	return m_point;
}
const ::std::valarray<double>& Canvas::get_Size() const {
	return m_vertex;
}

void Canvas::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	m_pD2dDeviceContext->SetTransform(::D2D1::IdentityMatrix());
	m_pD2dDeviceContext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
	m_pD2dDeviceContext->FillRectangle(
		::D2D1::RectF(
			static_cast<FLOAT>(::std::floor(point[0])),
			static_cast<FLOAT>(::std::floor(point[1])),
			static_cast<FLOAT>(::std::ceil(point[0] + vertex[0])),
			static_cast<FLOAT>(::std::ceil(point[1] + vertex[1]))
		),
		m_pPalette->get_pBrush(typeid(Canvas),
			[this](ID2D1Brush** ppBrush)->void {
				ID2D1SolidColorBrush* pBrush{ nullptr };
				::Windows::ThrowIfFailed(m_pD2dDeviceContext
					->CreateSolidColorBrush(
						m_pPalette
							->get_Theme()
								.Background,
						&pBrush
				), "Brush creation failed");
				*ppBrush = pBrush;
			})
	);
	m_pD2dDeviceContext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
}

void Canvas::Resize(const ::std::valarray<double>& vertex) {
	m_pD2dDeviceContext->SetTarget(nullptr);

	::Windows::ThrowIfFailed(m_pDxgiSwapChain->ResizeBuffers(
		0U,
		static_cast<UINT>(::std::ceil(vertex[0])),
		static_cast<UINT>(::std::ceil(vertex[1])),
		DXGI_FORMAT_UNKNOWN,
		0U
	), "Swap chain resize failed");

	ResetTarget();

	m_vertex = vertex;
}

void Canvas::ResetTarget() {
	::Microsoft::WRL::ComPtr<IDXGISurface> pDxgiBackBuffer{ nullptr };
	::Windows::ThrowIfFailed(m_pDxgiSwapChain->GetBuffer(
		0U, IID_PPV_ARGS(&pDxgiBackBuffer)
	), "Could not get back buffer");

	D2D1_BITMAP_PROPERTIES1 bitmapProperties{
		D2D1_PIXEL_FORMAT{
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED
		},
		0.F, 0.F,
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		nullptr
	};
	::Microsoft::WRL::ComPtr<ID2D1Bitmap1> pD2dBmp{ nullptr };
	::Windows::ThrowIfFailed(m_pD2dDeviceContext->CreateBitmapFromDxgiSurface(
		pDxgiBackBuffer.Get(),
		&bitmapProperties,
		&pD2dBmp
	), "Bitmap creation failed");

	m_pD2dDeviceContext->SetTarget(pD2dBmp.Get());
	m_pD2dDeviceContext->BeginDraw();
	m_pD2dDeviceContext->Clear(m_pPalette->get_Theme()
		.Background);
	::Windows::ThrowIfFailed(m_pD2dDeviceContext->EndDraw(
	), "Render failed");
	::Windows::ThrowIfFailed(m_pDxgiSwapChain->Present(
		1, 0
	), "Presentation failed");
}