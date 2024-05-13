#include "drawingengine.h"

#include "../../estd/guard.h"
#include "../wrappers.h"
#include "../../messages.h"

using namespace Voicemeeter::DeskBand::Windows::Presentation;

DrawingEngine::Context::~Context() {
	WaitForGpu();

	CloseHandle(m_fenceEvent);
}

void DrawingEngine::Context::Render() {
	WaitForGpu();

	ThrowIfFailed(m_pAllocator->Reset(
	), "Command allocator reset failed");
	ThrowIfFailed(m_pList->Reset(
		m_pAllocator.Get(),
		nullptr
	), "Command list reset failed");

	CD3DX12_RESOURCE_BARRIER presentToTarget{
		CD3DX12_RESOURCE_BARRIER::Transition(
			m_ppRenderTarget[m_frame].Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
	) };
	m_pList->ResourceBarrier(
		1U, &presentToTarget
	);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle{
		m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		static_cast<INT>(m_frame), m_rtvDescSize
	};
	m_pList->ClearRenderTargetView(
		rtvHandle,
		ColorF{ 44 / 255.F, 61 / 255.F, 77 / 255.F, 1.F },
		0U, nullptr
	);

	CD3DX12_RESOURCE_BARRIER targetToPresent{
		CD3DX12_RESOURCE_BARRIER::Transition(
			m_ppRenderTarget[m_frame].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		)
	};
	m_pList->ResourceBarrier(
		1U, &targetToPresent
	);

	ThrowIfFailed(m_pList->Close(
	), "Failed to close command list");
}
void DrawingEngine::Context::Update() const {
	ID3D12CommandList* ppList[]{ m_pList.Get() };
	m_pQueue->ExecuteCommandLists(_countof(ppList), ppList);

	DXGI_PRESENT_PARAMETERS params{
		0, NULL,
		NULL, NULL
	};
	ThrowIfFailed(m_pSwapChain->Present1(
		0U, 0U,
		&params
	), "Failed to present frame");
}
void DrawingEngine::Context::Resize(UINT w, UINT h) {
	WaitForGpu();

	for (UINT frame{ 0U }; frame < FrameCount; ++frame) {
		m_ppRenderTarget[frame].Reset();
	}

	UINT nodes[FrameCount]{ 0U, 0U };
	IUnknown* ppQueue[FrameCount]{ m_pQueue.Get(), m_pQueue.Get() };
	ThrowIfFailed(m_pSwapChain->ResizeBuffers1(
		FrameCount,
		w, h,
		DXGI_FORMAT_UNKNOWN,
		0U,
		nodes,
		ppQueue
	), "Buffer resizing failed");

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle{
		m_pRtvHeap->GetCPUDescriptorHandleForHeapStart()
	};
	for (UINT frame{ 0U }; frame < FrameCount; ++frame) {
		ThrowIfFailed(m_pSwapChain->GetBuffer(
			frame,
			IID_PPV_ARGS(&m_ppRenderTarget[frame])
		), "Failed to get swap chain buffer");
		m_pDevice->CreateRenderTargetView(
			m_ppRenderTarget[frame].Get(), nullptr,
			rtvHandle
		);
		rtvHandle.Offset(1, m_rtvDescSize);
	}

	m_fenceValue = 0U;
	m_frame = m_pSwapChain->GetCurrentBackBufferIndex();
}

void DrawingEngine::Context::WaitForGpu() {
	ThrowIfFailed(m_pQueue->Signal(
		m_pFence.Get(),
		m_fenceValue
	), "Fence signal failed");

	ThrowIfFailed(m_pFence->SetEventOnCompletion(
		m_fenceValue,
		m_fenceEvent
	), "Failed to set fence event");
	wWaitForSingleObject(
		m_fenceEvent,
		INFINITE
	);

	++m_fenceValue;
	m_frame = m_pSwapChain->GetCurrentBackBufferIndex();
}

DrawingEngine::DrawingEngine(const Style& style)
	: m_manifest{}
	, m_pFactory{ nullptr }
	, m_pDevice{ nullptr } {
	UINT flags{ 0U };

#ifndef NDEBUG
	ComPtr<ID3D12Debug3> pDebug{ nullptr };
	ThrowIfFailed(D3D12GetDebugInterface(
		IID_PPV_ARGS(&pDebug)
	), "Could not get debug interface");

	pDebug->EnableDebugLayer();

	flags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	
	ThrowIfFailed(CreateDXGIFactory2(
		flags,
		IID_PPV_ARGS(&m_pFactory)
	), "DXGI factory creation failed");

	ThrowIfFailed(D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_12_1,
		IID_PPV_ARGS(&m_pDevice)
	), "D3D12 device creation failed");
}

std::unique_ptr<DrawingEngine::Context> DrawingEngine::Initialize(HWND hWnd) {
	ComPtr<ID3D12CommandQueue> pQueue{ nullptr };
	ComPtr<ID3D12CommandAllocator> pAllocator{ nullptr };
	ComPtr<ID3D12GraphicsCommandList5> pList{ nullptr };
	ComPtr<ID3D12Fence1> pFence{ nullptr };
	HANDLE fenceEvent{ NULL };
	ComPtr<ID3D12DescriptorHeap> pRtvHeap{ nullptr };
	ComPtr<IDXGISwapChain4> pSwapChain{ nullptr };
	UINT rtvDescSize{ 0U };

	D3D12_COMMAND_QUEUE_DESC qDesc{
		D3D12_COMMAND_LIST_TYPE_DIRECT,			//.Type
		D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,	//.Priority
		D3D12_COMMAND_QUEUE_FLAG_NONE,			//.Flags
		0U										//.NodeMask
	};
	ThrowIfFailed(m_pDevice->CreateCommandQueue(
		&qDesc,
		IID_PPV_ARGS(&pQueue)
	), "D3D12 command queue creation failed");

	ThrowIfFailed(m_pDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&pAllocator)
	), "D3D12 command allocator creation failed");

	ThrowIfFailed(m_pDevice->CreateCommandList1(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		D3D12_COMMAND_LIST_FLAG_NONE,
		IID_PPV_ARGS(&pList)
	), "D3D12 command list creation failed");

	ThrowIfFailed(m_pDevice->CreateFence(
		0U,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&pFence)
	), "D3D12 fence creation failed");

	fenceEvent = wCreateEventW(
		NULL,
		FALSE,
		FALSE,
		NULL
	);
	auto fenceGuard = estd::make_guard([&fenceEvent]()->void {
		if (fenceEvent) {
			wCloseHandle(fenceEvent);
		}
	});

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,		//.Type
		FrameCount,							//.NumDescriptors
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,	//.Flags
		0U									//.NodeMask
	};
	ThrowIfFailed(m_pDevice->CreateDescriptorHeap(
		&rtvHeapDesc,
		IID_PPV_ARGS(&pRtvHeap)
	), "RTV descriptor heap creation failed");

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
		0U, 0U,								//.Width, .Height
		DXGI_FORMAT_R8G8B8A8_UNORM,			//.Format
		FALSE,								//.Stereo
		DXGI_SAMPLE_DESC{					//.SmapleDesc
			1U, 0U							//.Count, .Quality
		},
		DXGI_USAGE_RENDER_TARGET_OUTPUT,	//.BufferUsage
		FrameCount,							//.BufferCount
		DXGI_SCALING_NONE,					//.Scaling
		DXGI_SWAP_EFFECT_FLIP_DISCARD,		//.SwapEffect
		DXGI_ALPHA_MODE_IGNORE,				//.AlphaMode
		0U									//.Flags
	};
	ComPtr<IDXGISwapChain1> pSwapChain1{ nullptr };
	ThrowIfFailed(m_pFactory->CreateSwapChainForHwnd(
		pQueue.Get(),
		hWnd,
		&swapChainDesc, nullptr,
		nullptr,
		&pSwapChain1
	), "DXGI Swap chain creation failed");
	ThrowIfFailed(pSwapChain1.As(
		&pSwapChain
	), "DXGI Swap chain upgrade failed");

	ThrowIfFailed(m_pFactory->MakeWindowAssociation(
		hWnd,
		DXGI_MWA_NO_WINDOW_CHANGES
	), "Failed to disable DXGI window monitoring");

	rtvDescSize = m_pDevice->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV
	);

	std::unique_ptr<DrawingEngine::Context> ctx{ new DrawingEngine::Context{
		m_pDevice.Get(),
		pQueue.Get(),
		pAllocator.Get(),
		pList.Get(),
		pFence.Get(),
		fenceEvent,
		pRtvHeap.Get(),
		pSwapChain.Get(),
		rtvDescSize
	} };
	fenceEvent = NULL;

	return ctx;
}