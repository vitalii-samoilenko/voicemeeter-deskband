#include "drawingengine.h"

#include "../wrappers.h"

#include "atlas.h"
#include "../../vertex.h"
#include "../../pixel.h"

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
		m_pState.Get()
	), "Command list reset failed");

	frame.pList->SetGraphicsRootSignature(m_pRoot.Get());
	ID3D12DescriptorHeap* ppHeaps[] = { m_pSrvHeap.Get() };
	frame.pList->SetDescriptorHeaps(std::size(ppHeaps), ppHeaps);
	frame.pList->SetGraphicsRoot32BitConstants(0U, 4U, ColorF{ 112 / 255.F, 195 / 255.F, 153 / 255.F, 1.F }, 0U);
	frame.pList->SetGraphicsRootDescriptorTable(1, m_pSrvHeap->GetGPUDescriptorHandleForHeapStart());

	CD3DX12_RESOURCE_BARRIER presentToTarget{
		CD3DX12_RESOURCE_BARRIER::Transition(
			frame.pRenderTarget.Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		)
	};
	frame.pList->ResourceBarrier(
		1U, &presentToTarget
	);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle{
		m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		static_cast<INT>(m_frameIndex), m_rtvDescSize
	};
	frame.pList->OMSetRenderTargets(1U, &rtvHandle, FALSE, nullptr);
	frame.pList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	frame.pList->IASetVertexBuffers(0U, 1U, &m_vertView);

	frame.pList->ClearRenderTargetView(
		rtvHandle,
		ColorF{ 44 / 255.F, 61 / 255.F, 77 / 255.F, 1.F },
		0U, nullptr
	);
	D3D12_VIEWPORT v{ m_viewport };
	v.Width /= 3;
	D3D12_RECT s{ m_scissor };
	s.right /= 3;
	frame.pList->RSSetViewports(1U, &v);
	frame.pList->RSSetScissorRects(1U, &s);
	frame.pList->DrawInstanced(4U, 1U, 0U, 0U);
	v.TopLeftX += v.Width;
	s.left = s.right;
	s.right *= 2;
	frame.pList->RSSetViewports(1U, &v);
	frame.pList->RSSetScissorRects(1U, &s);
	frame.pList->DrawInstanced(4U, 1U, 0U, 0U);
	v.TopLeftX += v.Width;
	s.left = s.right;
	s.right = m_scissor.right;
	frame.pList->RSSetViewports(1U, &v);
	frame.pList->RSSetScissorRects(1U, &s);
	frame.pList->DrawInstanced(4U, 1U, 0U, 0U);

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
	w = max(w, 8U);
	h = max(h, 8U);

	m_viewport.Width = m_scissor.right = w;
	m_viewport.Height = m_scissor.bottom = h;
	m_viewport.MaxDepth = 1.F;

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

DrawingEngine::Context::Context(IDXGIFactory7* pFactory, ID3D12Device8* pDevice, HWND hWnd, IDCompositionVisual* pCompVisual)
	: m_pDevice{ pDevice }
	, m_pQueue{ nullptr }
	, m_pRoot{ nullptr }
	, m_pRtvHeap{ nullptr }
	, m_pSrvHeap{ nullptr }
	, m_rtvDescSize{ 0U }
	, m_pSwapChain{ nullptr }
	, m_pFrame{}
	, m_frameIndex{ 0U }
	, m_frameId{ 0U }
	, m_pFence{ nullptr }
	, m_fenceEvent{ NULL }
	, m_vertView{}
	, m_vert{ nullptr }
	, m_text{ nullptr }
	, m_viewport{}
	, m_scissor{} {
	D3D12_COMMAND_QUEUE_DESC qDesc{};
	ThrowIfFailed(pDevice->CreateCommandQueue(
		&qDesc,
		IID_PPV_ARGS(&m_pQueue)
	), "D3D12 command queue creation failed");

	CD3DX12_DESCRIPTOR_RANGE ranges[1]{};
	ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1U, 0U);

	CD3DX12_ROOT_PARAMETER params[2]{};
	params[0].InitAsConstants(4U, 0U, 0U, D3D12_SHADER_VISIBILITY_PIXEL);
	params[1].InitAsDescriptorTable(1U, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);

	D3D12_STATIC_SAMPLER_DESC sampler{};
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.F;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0U;
	sampler.RegisterSpace = 0U;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	CD3DX12_ROOT_SIGNATURE_DESC rootDesc{};
	rootDesc.Init(std::size(params), params, 1U, &sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ComPtr<ID3DBlob> signature{ nullptr };
	ComPtr<ID3DBlob> error{ nullptr };
	ThrowIfFailed(D3D12SerializeRootSignature(
		&rootDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&signature, &error
	), "Root signature serialization failed");
	ThrowIfFailed(pDevice->CreateRootSignature(
		0U,
		signature->GetBufferPointer(), signature->GetBufferSize(),
		IID_PPV_ARGS(&m_pRoot)
	), "Root signature creation failed");

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = FrameCount;
	ThrowIfFailed(pDevice->CreateDescriptorHeap(
		&rtvHeapDesc,
		IID_PPV_ARGS(&m_pRtvHeap)
	), "RTV descriptor heap creation failed");
	m_rtvDescSize = m_pDevice->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV
	);

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 8U;
	swapChainDesc.Height = 8U;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
	swapChainDesc.SampleDesc.Count = 1U;
	ComPtr<IDXGISwapChain1> pSwapChain1{ nullptr };
	ThrowIfFailed(pFactory->CreateSwapChainForComposition(
		m_pQueue.Get(),
		&swapChainDesc,
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

	ThrowIfFailed(pCompVisual->SetContent(
		m_pSwapChain.Get()
	), "Failed to set composition visual content");

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

	{
		Atlas a{};
		Atlas::Image out_a_0{ a.get_Image(Atlas::out_a_inact, 0) };
		Atlas::Image out_a_1{ a.get_Image(Atlas::out_a_inact, 1) };

		D3D12_SUBRESOURCE_DATA textureData[]{
			D3D12_SUBRESOURCE_DATA{
				out_a_0.pData,
				static_cast<LONG_PTR>(out_a_0.Width),
				static_cast<LONG_PTR>(out_a_0.Width * out_a_0.Height),
			},
			D3D12_SUBRESOURCE_DATA{
				out_a_1.pData,
				static_cast<LONG_PTR>(out_a_1.Width),
				static_cast<LONG_PTR>(out_a_1.Width * out_a_1.Height),
			}
		};

		D3D12_HEAP_PROPERTIES textureHeapProps{ D3D12_HEAP_TYPE_DEFAULT };
		D3D12_RESOURCE_DESC textureDesc{};
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Width = out_a_0.Width;
		textureDesc.Height = out_a_0.Height;
		textureDesc.DepthOrArraySize = 1U;
		textureDesc.MipLevels = std::size(textureData);
		textureDesc.Format = DXGI_FORMAT_R8_UNORM;
		textureDesc.SampleDesc.Count = 1U;
		ThrowIfFailed(pDevice->CreateCommittedResource(
			&textureHeapProps,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_text)
		), "Texture creation failed");

		ComPtr<ID3D12Resource> pTextureUploadHeap{ nullptr };
		D3D12_HEAP_PROPERTIES uploadHeapProps{ D3D12_HEAP_TYPE_UPLOAD };
		D3D12_RESOURCE_DESC uploadHeapDesc{
			CD3DX12_RESOURCE_DESC::Buffer(GetRequiredIntermediateSize(m_text.Get(), 0, std::size(textureData)))
		};
		ThrowIfFailed(pDevice->CreateCommittedResource(
			&uploadHeapProps,
			D3D12_HEAP_FLAG_NONE,
			&uploadHeapDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&pTextureUploadHeap)
		), "Texture upload heap creation failed");

		Frame& frame{ m_pFrame[0] };
		frame.Id = ++m_frameIndex;

		ThrowIfFailed(frame.pAllocator->Reset(
		), "Command allocator reset failed");
		ThrowIfFailed(frame.pList->Reset(
			frame.pAllocator.Get(),
			nullptr
		), "Command list reset failed");

		UpdateSubresources(frame.pList.Get(), m_text.Get(), pTextureUploadHeap.Get(), 0, 0, std::size(textureData), textureData);

		CD3DX12_RESOURCE_BARRIER copyToShader{
			CD3DX12_RESOURCE_BARRIER::Transition(
				m_text.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
			)
		};
		frame.pList->ResourceBarrier(1U, &copyToShader);

		ThrowIfFailed(frame.pList->Close(
		), "Failed to close command list");

		ID3D12CommandList* ppList[]{ frame.pList.Get() };
		m_pQueue->ExecuteCommandLists(std::size(ppList), ppList);
		m_pQueue->Signal(m_pFence.Get(), frame.Id);

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(pDevice->CreateDescriptorHeap(
			&srvHeapDesc,
			IID_PPV_ARGS(&m_pSrvHeap)
		), "SRV descriptor heap creation failed");

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Texture2D.MipLevels = std::size(textureData);
		pDevice->CreateShaderResourceView(m_text.Get(), &srvDesc, m_pSrvHeap->GetCPUDescriptorHandleForHeapStart());

		struct Vertex {
			FLOAT xyz[3];
			FLOAT uv[2];
		};
		Vertex v[4]{
			Vertex{
				{ -1.F, 1.F, 0.F },
				{ 0.F, 0.F }
			},
			Vertex{
				{ 1.F, 1.F, 0.F },
				{ 1.F, 0.F }
			},
			Vertex{
				{ -1.F, -1.F, 0.F },
				{ 0.F, 1.F }
			},
			Vertex{
				{ 1.F, -1.F, 0.F },
				{ 1.F, 1.F }
			},
		};
		size_t vSize{ sizeof(v) };

		// Note: using upload heaps to transfer static data like vert buffers is not 
		// recommended. Every time the GPU needs it, the upload heap will be marshalled 
		// over. Please read up on Default Heap usage. An upload heap is used here for 
		// code simplicity and because there are very few verts to actually transfer.
		CD3DX12_HEAP_PROPERTIES uploadProps{ D3D12_HEAP_TYPE_UPLOAD };
		CD3DX12_RESOURCE_DESC bufferDesc{
			CD3DX12_RESOURCE_DESC::Buffer(vSize)
		};
		ThrowIfFailed(pDevice->CreateCommittedResource(
			&uploadProps,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_vert)
		), "Vertex buffer creation failed");

		void* pVBegin;
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_vert->Map(
			0U, &readRange,
			&pVBegin
		), "Failed to map vertex buffer");
		memcpy(pVBegin, v, vSize);
		m_vert->Unmap(0U, nullptr);

		m_vertView.BufferLocation = m_vert->GetGPUVirtualAddress();
		m_vertView.StrideInBytes = sizeof(Vertex);
		m_vertView.SizeInBytes = vSize;

		// Define the vertex input layout.
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[]{
			{ "POSITION", 0U, DXGI_FORMAT_R32G32B32_FLOAT, 0U, 0U, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0U },
			{ "TEXCOORD", 0U, DXGI_FORMAT_R32G32_FLOAT, 0U, 12U, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0U }
		};

		const D3D12_BLEND_DESC AlphaBlend{
			FALSE, // AlphaToCoverageEnable
			FALSE, // IndependentBlendEnable
			{
				TRUE, // BlendEnable
				FALSE, // LogicOpEnable
				D3D12_BLEND_ONE, // SrcBlend
				D3D12_BLEND_INV_SRC_ALPHA, // DestBlend
				D3D12_BLEND_OP_ADD, // BlendOp
				D3D12_BLEND_ONE, // SrcBlendAlpha
				D3D12_BLEND_INV_SRC_ALPHA, // DestBlendAlpha
				D3D12_BLEND_OP_ADD, // BlendOpAlpha
				D3D12_LOGIC_OP_CLEAR, // LogicOp
				D3D12_COLOR_WRITE_ENABLE_ALL // RenderTargetWriteMask
			}
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_pRoot.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(VS_MAIN, std::size(VS_MAIN));
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(PS_MAIN, std::size(PS_MAIN));
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		psoDesc.BlendState = AlphaBlend;
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		ThrowIfFailed(pDevice->CreateGraphicsPipelineState(
			&psoDesc,
			IID_PPV_ARGS(&m_pState)
		), "Pipeline state creation failed");

		WaitForFrame(frame.Id);
	}
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