#include <d3d11_4.h>

#include "Palette.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dcomp")

using namespace ::Voicemeeter::UI::D2D::Graphics;

void Atlas::Rescale(const ::std::valarray<double>& scale) {
	m_pAtlas.reset(new ::Voicemeeter::Atlas::Cherry{ m_horizontal, scale });

	::Windows::ThrowIfFailed(m_palette.get_Instrumentation()
		.get_pDeviceContext()
			->CreateBitmapFromWicBitmap(
				m_pAtlas->get_pBitmap(),
				m_palette.get_Instrumentation()
					.get_ppBitmap()
	), "Bitmap creation failed");
}
void Atlas::Fill(
	const ::std::valarray<double>& point,
	const ::std::valarray<double>& vertex,
	const ::std::valarray<double>& maskPoint,
	const ::std::valarray<double>& color,
	bool blend) {
	using RGBA = ::Voicemeeter::UI::Cherry::Graphics::Theme::RGBA;

	constexpr double AAEPS{ 0.5 - ::std::numeric_limits<double>::epsilon() };

	if (blend) {
		m_palette.get_Instrumentation()
			.get_pDeviceContext()
				->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
	}
	m_palette.get_Instrumentation()
		.get_pBrush()
			->SetColor(::D2D1::ColorF(
				static_cast<FLOAT>(color[RGBA::red]),
				static_cast<FLOAT>(color[RGBA::green]),
				static_cast<FLOAT>(color[RGBA::blue]),
				static_cast<FLOAT>(color[RGBA::alpha])));
	m_palette.get_Instrumentation()
		.get_pDeviceContext()
			->FillOpacityMask(
				m_palette.get_Instrumentation()
					.get_pBitmap(),
				m_palette.get_Instrumentation()
					.get_pBrush(),
				::D2D1::RectF(
					static_cast<FLOAT>(point[0]),
					static_cast<FLOAT>(point[1]),
					static_cast<FLOAT>(point[0] + vertex[0] + AAEPS),
					static_cast<FLOAT>(point[1] + vertex[1] + AAEPS)),
				::D2D1::RectF(
					static_cast<FLOAT>(maskPoint[0]),
					static_cast<FLOAT>(maskPoint[1]),
					static_cast<FLOAT>(maskPoint[0] + vertex[0] + AAEPS),
					static_cast<FLOAT>(maskPoint[1] + vertex[1] + AAEPS)));
	if (blend) {
		m_palette.get_Instrumentation()
			.get_pDeviceContext()
				->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
	}
}

Instrumentation::Instrumentation(
	HWND hWnd
) : m_pD2dFactory{ nullptr }
  , m_pDeviceContext{ nullptr }
  , m_pSwapChain{ nullptr }
  , m_pCompositionTarget{ nullptr }
  , m_pBrush{ nullptr }
  , m_pBitmap{ nullptr } {
	::Windows::ThrowIfFailed(CoInitialize(
		NULL
	), "COM initialization failed");

	::Windows::ThrowIfFailed(D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
#ifndef NDEBUG
		D2D1_FACTORY_OPTIONS{
			D2D1_DEBUG_LEVEL_INFORMATION
		},
#endif // !NDEBUG
		m_pD2dFactory.ReleaseAndGetAddressOf()
	), "Direct2D factory creation failed");

	D3D_FEATURE_LEVEL pFeatureLevel[]{
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
		pFeatureLevel, ARRAYSIZE(pFeatureLevel), D3D11_SDK_VERSION,
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
		&m_pDeviceContext
	), "Direct2D device context creation failed");
	m_pDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	m_pDeviceContext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);

	::Microsoft::WRL::ComPtr<IDXGIAdapter> pDxgiAdapter{ nullptr };
	::Windows::ThrowIfFailed(pDxgiDevice->GetAdapter(
		&pDxgiAdapter
	), "Could not get DXGI adapter");

	::Microsoft::WRL::ComPtr<IDXGIFactory2> pDxgiFactory{ nullptr };
	::Windows::ThrowIfFailed(pDxgiAdapter->GetParent(
		IID_PPV_ARGS(&pDxgiFactory)
	), "Could not get DXGI factory");

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
		8U, 8U,
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
		&m_pSwapChain
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
		m_pSwapChain.Get()
	), "Could not set swap chain content");

	::Windows::ThrowIfFailed(m_pCompositionTarget->SetRoot(
		pCompositionVisual.Get()
	), "Could not set composition target root");

	::Windows::ThrowIfFailed(pCompositionDevice->Commit(
	), "Could not commit composition device");

	::Windows::ThrowIfFailed(m_pDeviceContext->CreateSolidColorBrush(
		::D2D1::ColorF(1.F, 1.F, 1.F, 1.F),
		&m_pBrush
	), "Brush creation failed");
}