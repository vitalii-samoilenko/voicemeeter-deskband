#include "drawingengine.h"

#include "../wrappers.h"

using namespace Voicemeeter::DeskBand::Windows::Presentation;

DrawingEngine::Context::~Context() {
	WaitForFrame(m_frameId);

	wCloseHandle(m_fenceEvent);
}

void DrawingEngine::Context::Render() {
	Frame& frame{ m_pFrame[m_frameIndex] };

	WaitForFrame(frame.Id);
	frame.Id = ++m_frameId;

	ThrowIfFailed(frame.pAllocator->Reset(
	), "Command allocator reset failed");
	ThrowIfFailed(frame.pList->Reset(
		frame.pAllocator.Get(),
		nullptr
	), "Command list reset failed");

	CD3DX12_RESOURCE_BARRIER presentToTarget{
		CD3DX12_RESOURCE_BARRIER::Transition(
			frame.pRenderTarget.Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
	) };
	frame.pList->ResourceBarrier(
		1U, &presentToTarget
	);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle{
		m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		static_cast<INT>(m_frameIndex), m_rtvDescSize
	};
	frame.pList->ClearRenderTargetView(
		rtvHandle,
		ColorF{ 44 / 255.F, 61 / 255.F, 77 / 255.F, 1.F },
		0U, nullptr
	);

	CD3DX12_RESOURCE_BARRIER targetToPresent{
		CD3DX12_RESOURCE_BARRIER::Transition(
			frame.pRenderTarget.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		)
	};
	frame.pList->ResourceBarrier(
		1U, &targetToPresent
	);

	ThrowIfFailed(frame.pList->Close(
	), "Failed to close command list");
}
void DrawingEngine::Context::Update() {
	Frame& frame{ m_pFrame[m_frameIndex] };

	ID3D12CommandList* ppList[]{ frame.pList.Get()};
	m_pQueue->ExecuteCommandLists(std::size(ppList), ppList);

	DXGI_PRESENT_PARAMETERS params{
		0, NULL,
		NULL, NULL
	};
	ThrowIfFailed(m_pSwapChain->Present1(
		0U, 0U,
		&params
	), "Failed to present frame");
	m_frameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

	m_pQueue->Signal(m_pFence.Get(), frame.Id);
}
void DrawingEngine::Context::Resize(UINT w, UINT h) {
	WaitForFrame(m_frameId);

	for (Frame& frame : m_pFrame) {
		frame.pRenderTarget.Reset();
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
	for (m_frameIndex = 0; m_frameIndex < FrameCount; ++m_frameIndex) {
		ThrowIfFailed(m_pSwapChain->GetBuffer(
			m_frameIndex,
			IID_PPV_ARGS(&m_pFrame[m_frameIndex].pRenderTarget)
		), "Failed to get swap chain buffer");
		m_pDevice->CreateRenderTargetView(
			m_pFrame[m_frameIndex].pRenderTarget.Get(), nullptr,
			rtvHandle
		);
		rtvHandle.Offset(1, m_rtvDescSize);
	}

	m_frameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
}

DrawingEngine::Context::Context(IDXGIFactory7* pFactory, ID3D12Device8* pDevice, HWND hWnd)
	: m_pDevice{ pDevice }
	, m_pQueue{ nullptr }
	, m_pRtvHeap{ nullptr }
	, m_rtvDescSize{ 0U }
	, m_pSwapChain{ nullptr }
	, m_pFrame{}
	, m_frameIndex{ 0U }
	, m_frameId{ 0U }
	, m_pFence{ nullptr }
	, m_fenceEvent{ NULL } {
	D3D12_COMMAND_QUEUE_DESC qDesc{
		D3D12_COMMAND_LIST_TYPE_DIRECT,			//.Type
		D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,	//.Priority
		D3D12_COMMAND_QUEUE_FLAG_NONE,			//.Flags
		0U										//.NodeMask
	};
	ThrowIfFailed(pDevice->CreateCommandQueue(
		&qDesc,
		IID_PPV_ARGS(&m_pQueue)
	), "D3D12 command queue creation failed");

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,		//.Type
		FrameCount,							//.NumDescriptors
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,	//.Flags
		0U									//.NodeMask
	};
	ThrowIfFailed(pDevice->CreateDescriptorHeap(
		&rtvHeapDesc,
		IID_PPV_ARGS(&m_pRtvHeap)
	), "RTV descriptor heap creation failed");
	m_rtvDescSize = m_pDevice->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV
	);

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
	ThrowIfFailed(pFactory->CreateSwapChainForHwnd(
		m_pQueue.Get(),
		hWnd,
		&swapChainDesc, nullptr,
		nullptr,
		&pSwapChain1
	), "DXGI Swap chain creation failed");
	ThrowIfFailed(pSwapChain1.As(
		&m_pSwapChain
	), "DXGI Swap chain upgrade failed");

	ThrowIfFailed(pFactory->MakeWindowAssociation(
		hWnd,
		DXGI_MWA_NO_WINDOW_CHANGES
	), "Failed to disable DXGI window monitoring");

	for (Frame& frame : m_pFrame) {
		ThrowIfFailed(pDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&frame.pAllocator)
		), "D3D12 command allocator creation failed");

		ThrowIfFailed(pDevice->CreateCommandList1(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			D3D12_COMMAND_LIST_FLAG_NONE,
			IID_PPV_ARGS(&frame.pList)
		), "D3D12 command list creation failed");
	}

	ThrowIfFailed(pDevice->CreateFence(
		0U,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&m_pFence)
	), "D3D12 fence creation failed");

	m_fenceEvent = wCreateEventW(
		NULL,
		FALSE,
		FALSE,
		NULL
	);
}

void DrawingEngine::Context::WaitForFrame(UINT64 id) {
	if (m_pFence->GetCompletedValue() < id) {
		ThrowIfFailed(m_pFence->SetEventOnCompletion(
			id,
			m_fenceEvent
		), "Failed to set fence event");
		wWaitForSingleObject(
			m_fenceEvent,
			INFINITE
		);
	}
}