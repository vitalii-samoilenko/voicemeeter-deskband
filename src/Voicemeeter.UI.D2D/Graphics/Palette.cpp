#include <array>
#include <cmath>

#include <d3d11_4.h>

#include "Windows/Wrappers.h"

#include "Palette.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dcomp")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "uxtheme")

using namespace ::Voicemeeter::UI::D2D::Graphics;

Palette::Palette(
	HWND hWnd,
	const Theme& theme
) : m_theme{ theme }
  , m_flatteringTolerance{ ::D2D1::ComputeFlatteningTolerance(::D2D1::IdentityMatrix(), 96.F, 96.F, 4.F) }
  , m_pDwFactory{ nullptr }
  , m_pD2dFactory{ nullptr }
  , m_pD2dDeviceContext{ nullptr }
  , m_pDxgiSwapChain{ nullptr }
  , m_pCompositionTarget{ nullptr }
  , m_cpTextFormat{}
  , m_cpTextLayout{}
  , m_cpBrush{}
  , m_cpGeometry{}
  , m_now{}
  , m_past{}
  , m_queue{} {
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
}

IDWriteTextFormat* Palette::get_pTextFormat(const ::std::wstring& fontFamily) const {
	::Microsoft::WRL::ComPtr<IDWriteTextFormat>& pTextFormat{
		m_cpTextFormat[fontFamily]
	};
	if (!pTextFormat) {
		::Windows::ThrowIfFailed(m_pDwFactory
			->CreateTextFormat(
				fontFamily.c_str(),
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				16,
				L"", //locale
				&pTextFormat
		), "Text format creation failed");
	}
	return pTextFormat.Get();
}
IDWriteTextLayout* Palette::get_pTextLayout(const ::std::wstring& text, const ::std::wstring& fontFamily) const {
	::Microsoft::WRL::ComPtr<IDWriteTextLayout>& pTextLayout{
		m_cpTextLayout[text]
	};
	if (!pTextLayout) {
		::Windows::ThrowIfFailed(m_pDwFactory
			->CreateTextLayout(
				text.c_str(),
				static_cast<UINT32>(text.length()),
				get_pTextFormat(fontFamily),
				::std::numeric_limits<FLOAT>::max(),
				::std::numeric_limits<FLOAT>::max(),
				&pTextLayout
		), "Text layout creation failed");
	}
	return pTextLayout.Get();
}