#include "drawingengine.h"

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
			m_pRenderTargets[m_frame].Get(),
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
	const FLOAT bkg[]{ 44 / 255.F, 61 / 255.F, 77 / 255.F, 1.F };
	m_pList->ClearRenderTargetView(
		rtvHandle,
		bkg,
		0U, nullptr
	);

	CD3DX12_RESOURCE_BARRIER targetToPresent{
		CD3DX12_RESOURCE_BARRIER::Transition(
			m_pRenderTargets[m_frame].Get(),
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
		m_pRenderTargets[frame].Reset();
	}

	UINT nodes[FrameCount]{ 0U, 0U };
	IUnknown* pQueues[FrameCount]{ m_pQueue.Get(), m_pQueue.Get() };
	ThrowIfFailed(m_pSwapChain->ResizeBuffers1(
		FrameCount,
		w, h,
		DXGI_FORMAT_UNKNOWN,
		0U,
		nodes,
		pQueues
	), "Buffer resizing failed");

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle{
		m_pRtvHeap->GetCPUDescriptorHandleForHeapStart()
	};
	for (UINT frame{ 0U }; frame < FrameCount; ++frame) {
		ThrowIfFailed(m_pSwapChain->GetBuffer(
			frame,
			IID_PPV_ARGS(&m_pRenderTargets[frame])
		), "Failed to get swap chain buffer");
		m_pDevice->CreateRenderTargetView(
			m_pRenderTargets[frame].Get(), nullptr,
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
	ComPtr<IDXGIAdapter4> pAdapter{ nullptr };
	ThrowIfFailed(D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_12_1,
		IID_PPV_ARGS(&m_pDevice)
	), "D3D12 device creation failed");
}

std::unique_ptr<DrawingEngine::Context> DrawingEngine::Initialize(HWND hWnd) {
	std::unique_ptr<DrawingEngine::Context> ctx{ new DrawingEngine::Context{} };

	D3D12_COMMAND_QUEUE_DESC qDesc{
		D3D12_COMMAND_LIST_TYPE_DIRECT,			//.Type
		D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,	//.Priority
		D3D12_COMMAND_QUEUE_FLAG_NONE,			//.Flags
		0U										//.NodeMask
	};
	ThrowIfFailed(m_pDevice->CreateCommandQueue(
		&qDesc,
		IID_PPV_ARGS(&ctx->m_pQueue)
	), "D3D12 command queue creation failed");

	ThrowIfFailed(m_pDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&ctx->m_pAllocator)
	), "D3D12 command allocator creation failed");

	ThrowIfFailed(m_pDevice->CreateCommandList1(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		D3D12_COMMAND_LIST_FLAG_NONE,
		IID_PPV_ARGS(&ctx->m_pList)
	), "D3D12 command list creation failed");

	ThrowIfFailed(m_pDevice->CreateFence(
		0U,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&ctx->m_pFence)
	), "D3D12 fence creation failed");

	ctx->m_fenceEvent = wCreateEventW(
		NULL,
		FALSE,
		FALSE,
		NULL
	);

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,		//.Type
		FrameCount,							//.NumDescriptors
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,	//.Flags
		0U									//.NodeMask
	};
	ThrowIfFailed(m_pDevice->CreateDescriptorHeap(
		&rtvHeapDesc,
		IID_PPV_ARGS(&ctx->m_pRtvHeap)
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
		DXGI_ALPHA_MODE_UNSPECIFIED,		//.AlphaMode
		0U									//.Flags
	};
	ComPtr<IDXGISwapChain1> pSwapChain{ nullptr };
	ThrowIfFailed(m_pFactory->CreateSwapChainForHwnd(
		ctx->m_pQueue.Get(),
		hWnd,
		&swapChainDesc, nullptr,
		nullptr,
		&pSwapChain
	), "DXGI Swap chain creation failed");
	ThrowIfFailed(pSwapChain.As(
		&ctx->m_pSwapChain
	), "DXGI Swap chain upgrade failed");

	//ThrowIfFailed(m_pFactory->MakeWindowAssociation(
	//	hWnd,
	//	DXGI_MWA_NO_WINDOW_CHANGES
	//), "Failed to disable DXGI window monitoring");

	ctx->m_rtvDescSize = m_pDevice->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV
	);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle{
		ctx->m_pRtvHeap->GetCPUDescriptorHandleForHeapStart()
	};
	for (UINT frame{ 0U }; frame < FrameCount; ++frame) {
		ThrowIfFailed(ctx->m_pSwapChain->GetBuffer(
			frame,
			IID_PPV_ARGS(&ctx->m_pRenderTargets[frame])
		), "Failed to get swap chain buffer");
		m_pDevice->CreateRenderTargetView(
			ctx->m_pRenderTargets[frame].Get(), nullptr,
			rtvHandle
		);
		rtvHandle.Offset(1, ctx->m_rtvDescSize);
	}

	ctx->m_frame = ctx->m_pSwapChain->GetCurrentBackBufferIndex();

	m_pDevice.CopyTo(&ctx->m_pDevice);

	return ctx;
}