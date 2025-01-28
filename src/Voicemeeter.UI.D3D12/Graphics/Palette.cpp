#include "Palette.h"

#pragma comment(lib, "dcomp")

using namespace ::Voicemeeter::UI::D3D12::Graphics;

void Atlas::Rescale(const ::std::valarray<double>& scale) {
	m_pAtlas.reset(new ::Voicemeeter::Atlas::Cherry{ m_horizontal, scale });
}
void Atlas::Fill(
	const ::std::valarray<double>& point,
	const ::std::valarray<double>& vertex,
	const ::std::valarray<double>& maskPoint,
	const ::std::valarray<double>& color,
	bool blend) {
	using RGBA = ::Voicemeeter::UI::Cherry::Graphics::Theme::RGBA;

	constexpr double AAEPS{ 0.5 - ::std::numeric_limits<double>::epsilon() };
}

Instrumentation::Instrumentation(
	HWND hWnd
) : m_pD3dDevice{ nullptr }
  , m_pSwapChain{ nullptr }
  , m_pCommandQueue{ nullptr }
  , m_pRtvHeap{ nullptr }
  , m_pCompositionTarget{ nullptr }
  , m_frame{ 0 }
  , m_chRtv{}
  , m_cpRenderTarget{ nullptr, nullptr }
  , m_cpCommandAllocator{ nullptr, nullptr }
  , m_cpCommandList{ nullptr, nullptr }
  , m_cpFence{ nullptr, nullptr }
  , m_chEvent{ NULL, NULL }
  , m_cCount{ 0ULL, 0ULL } {
	::Windows::ThrowIfFailed(CoInitialize(
		NULL
	), "COM initialization failed");

	UINT dxgiFactoryFlags{ 0U };

#ifndef NDEBUG
	::Microsoft::WRL::ComPtr<ID3D12Debug> pD3dDebug{ nullptr };
	::Windows::ThrowIfFailed(D3D12GetDebugInterface(
		IID_PPV_ARGS(&pD3dDebug)
	), "Could not get debug interface");

	pD3dDebug->EnableDebugLayer();

	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif // !NDEBUG

	::Microsoft::WRL::ComPtr<IDXGIFactory7> pDxgiFactory{ nullptr };
	::Windows::ThrowIfFailed(CreateDXGIFactory2(
		dxgiFactoryFlags,
		IID_PPV_ARGS(&pDxgiFactory)
	), "DXGI factory creation failed");

	::Windows::ThrowIfFailed(pDxgiFactory->MakeWindowAssociation(
		hWnd,
		DXGI_MWA_NO_ALT_ENTER
	), "Failed to disable fullscreen transition");

	::Microsoft::WRL::ComPtr<IDXGIAdapter4> pDxgiAdapter{ nullptr };
	::Windows::ThrowIfFailed(pDxgiFactory->EnumAdapterByGpuPreference(
		0U,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&pDxgiAdapter)
	), "Could not get DXGI adapter");

	::Windows::ThrowIfFailed(D3D12CreateDevice(
		pDxgiAdapter.Get(),
		D3D_FEATURE_LEVEL_12_2,
		IID_PPV_ARGS(&m_pD3dDevice)
	), "D3D12 device creation failed");

	D3D12_COMMAND_QUEUE_DESC queueDesc{
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		D3D12_COMMAND_QUEUE_PRIORITY_GLOBAL_REALTIME,
		D3D12_COMMAND_QUEUE_FLAG_NONE,
		0U
	};
	::Windows::ThrowIfFailed(m_pD3dDevice->CreateCommandQueue(
		&queueDesc,
		IID_PPV_ARGS(&m_pCommandQueue)
	), "Command queue creation failed");

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
		8U, 8U,
		DXGI_FORMAT_B8G8R8A8_UNORM,
		FALSE,
		DXGI_SAMPLE_DESC{
			1U, 0U
		},
		DXGI_USAGE_RENDER_TARGET_OUTPUT, FRAME_COUNT,
		DXGI_SCALING_STRETCH,
		DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
		DXGI_ALPHA_MODE_PREMULTIPLIED,
		0U
	};
	::Microsoft::WRL::ComPtr<IDXGISwapChain1> pSwapChain{ nullptr };
	::Windows::ThrowIfFailed(pDxgiFactory->CreateSwapChainForComposition(
		m_pCommandQueue.Get(),
		&swapChainDesc,
		nullptr,
		&pSwapChain
	), "DXGI swap chain creation failed");

	::Windows::ThrowIfFailed(pSwapChain.As(
		&m_pSwapChain
	), "Could not get swap chain");
	m_frame = m_pSwapChain->GetCurrentBackBufferIndex();

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV, FRAME_COUNT,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0U
	};
	::Windows::ThrowIfFailed(m_pD3dDevice->CreateDescriptorHeap(
		&rtvHeapDesc,
		IID_PPV_ARGS(&m_pRtvHeap)
	), "RTV heap descriptor creation failed");

	UINT rtvDescriptorSize{ m_pD3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) };
	D3D12_CPU_DESCRIPTOR_HANDLE hRtv{ m_pRtvHeap->GetCPUDescriptorHandleForHeapStart() };

	for (size_t i{ 0 }; i < FRAME_COUNT; ++i) {
		m_chRtv[i].ptr = SIZE_T(INT64(hRtv.ptr) + INT64(i * rtvDescriptorSize));
		::Windows::ThrowIfFailed(m_pSwapChain->GetBuffer(
			i,
			IID_PPV_ARGS(&m_cpRenderTarget[i])
		), "Failed to get swap chain buffer");
		m_pD3dDevice->CreateRenderTargetView(m_cpRenderTarget[i].Get(), nullptr, m_chRtv[i]);

		::Windows::ThrowIfFailed(m_pD3dDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_cpCommandAllocator[i])
		), "Command allocator creation failed");

		::Windows::ThrowIfFailed(m_pD3dDevice->CreateCommandList1(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT,
			D3D12_COMMAND_LIST_FLAG_NONE,
			IID_PPV_ARGS(&m_cpCommandList[i])
		), "Command list creation failed");

		::Windows::ThrowIfFailed(m_pD3dDevice->CreateFence(
			m_cCount[i],
			D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(&m_cpFence[i])
		), "Fence creation failed");

		m_chEvent[i] = ::Windows::wCreateEventW(nullptr, FALSE, FALSE, nullptr);
	}

	::Microsoft::WRL::ComPtr<IDXGIDevice1> pDxgiDevice{ nullptr };
	::Windows::ThrowIfFailed(m_pD3dDevice.As(
		&pDxgiDevice
	), "Could not get DXGI device");

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
}

Instrumentation::~Instrumentation() {
	for (HANDLE& hEvent : m_chEvent) {
		CloseHandle(hEvent);
	}
}