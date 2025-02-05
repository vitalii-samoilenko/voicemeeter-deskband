#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include <cmath>
#include <vector>

#include "estd/memory.h"

#include <d3dcompiler.h>

#include "../Shaders.h"

#include "Palette.h"

#pragma comment(lib, "d3d12")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dcomp")
#pragma comment(lib, "dxgi")

using namespace ::Voicemeeter::UI::D3D12::Graphics;

void Atlas::Rescale(const ::std::valarray<double>& scale) {
	::Windows::ThrowIfFailed(m_palette.get_Instrumentation()
		.get_pCommandAllocator()
			->Reset(
	), "Command allocator reset failed");
	::Windows::ThrowIfFailed(m_palette.get_Instrumentation()
		.get_pCommandList()
			->Reset(
				m_palette.get_Instrumentation()
					.get_pCommandAllocator(),
				nullptr
	), "Command list reset failed");

	m_pAtlas.reset(new ::Voicemeeter::Atlas::Cherry{ m_horizontal, scale });

	::Microsoft::WRL::ComPtr<IWICBitmapLock> pLock{ nullptr };
	::Windows::ThrowIfFailed(m_pAtlas->get_pBitmap()
		->Lock(
			nullptr,
			WICBitmapLockRead,
			&pLock
	), "Bitmap lock failed");

	UINT size{ 0U };
	WICInProcPointer pSrc{ nullptr };
	::Windows::ThrowIfFailed(pLock->GetDataPointer(
		&size, &pSrc
	), "Failed to get bitmap data");

	D3D12_HEAP_PROPERTIES pHeap{
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0U, 0U
	};
	D3D12_RESOURCE_DESC dResource{
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0ULL,
		m_pAtlas->get_Width(), m_pAtlas->get_Height(), 1U, 1U,
#ifndef NDEBUG
		DXGI_FORMAT_R8G8B8A8_UNORM,
#else
		DXGI_FORMAT_A8_UNORM,
#endif // !NDEBUG
		DXGI_SAMPLE_DESC{
			1U, 0U
		},
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_NONE
	};
	::Windows::ThrowIfFailed(m_palette.get_Instrumentation()
		.get_pD3dDevice()
			->CreateCommittedResource(
				&pHeap, D3D12_HEAP_FLAG_NONE, &dResource,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(m_palette.get_Instrumentation()
					.get_ppTexture())
	), "Texture creation failed");

#ifndef NDEBUG
	static constexpr UINT64 PixelSize{ 4ULL };
#else
	static constexpr UINT64 PixelSize{ 1ULL };
#endif // !NDEBUG

	UINT rowSize{ m_pAtlas->get_Width() * PixelSize };
	UINT remainder{ rowSize % 4ULL };
	UINT srcRowPitch{
		(remainder
			? rowSize - remainder + 4ULL
			: rowSize)
	};
	remainder = rowSize % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT;
	UINT dstRowPitch{
		(remainder
			? rowSize - remainder + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT
			: rowSize)
	};

	pHeap.Type = D3D12_HEAP_TYPE_UPLOAD;
	dResource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	dResource.Width = dstRowPitch * m_pAtlas->get_Height();
	dResource.Height = 1U;
	dResource.Format = DXGI_FORMAT_UNKNOWN;
	dResource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	::Microsoft::WRL::ComPtr<ID3D12Resource> pUploadBuffer{ nullptr };
	::Windows::ThrowIfFailed(m_palette.get_Instrumentation()
		.get_pD3dDevice()
			->CreateCommittedResource(
				&pHeap, D3D12_HEAP_FLAG_NONE, &dResource,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pUploadBuffer)
	), "Upload buffer creation failed");

	D3D12_RANGE range{
		0ULL, 0ULL
	};
	BYTE* pDst{ nullptr };
	::Windows::ThrowIfFailed(pUploadBuffer->Map(
		0U, &range,
		reinterpret_cast<void**>(&pDst)
	), "Upload buffer map failed");
	for (UINT row{ 0U }; row < m_pAtlas->get_Height(); ++row) {
		memcpy(pDst + dstRowPitch * row, pSrc + srcRowPitch * row, rowSize);
	}
	pUploadBuffer->Unmap(0U, nullptr);

	D3D12_TEXTURE_COPY_LOCATION lSrc{
		pUploadBuffer.Get(),
		D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT,
		D3D12_PLACED_SUBRESOURCE_FOOTPRINT{
			0ULL,
			D3D12_SUBRESOURCE_FOOTPRINT{
#ifndef NDEBUG
				DXGI_FORMAT_R8G8B8A8_UNORM,
#else
				DXGI_FORMAT_A8_UNORM,
#endif // !NDEBUG
				m_pAtlas->get_Width(), m_pAtlas->get_Height(), 1U,
				dstRowPitch
			}
		}
	};
	D3D12_TEXTURE_COPY_LOCATION lDst{
		m_palette.get_Instrumentation()
			.get_pTexture(),
		D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX
	};
	lDst.SubresourceIndex = 0U;

	m_palette.get_Instrumentation()
		.get_pCommandList()
			->CopyTextureRegion(
				&lDst,
				0U, 0U, 0U,
				&lSrc,
				nullptr);
	D3D12_RESOURCE_BARRIER barrier{
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_TRANSITION_BARRIER{
			m_palette.get_Instrumentation()
				.get_pTexture(),
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
		}
	};
	m_palette.get_Instrumentation()
		.get_pCommandList()
			->ResourceBarrier(1U, &barrier);

	::Windows::ThrowIfFailed(m_palette.get_Instrumentation()
		.get_pCommandList()
			->Close(
	), "Command list close failed");
	::std::array<ID3D12CommandList*, 1> cpCommandList{
		m_palette.get_Instrumentation()
			.get_pCommandList()
	};
	m_palette.get_Instrumentation()
		.get_pCommandQueue()
			->ExecuteCommandLists(
				static_cast<UINT>(cpCommandList.size()),
				cpCommandList.data());
	m_palette.get_Instrumentation()
		.get_pCommandQueue()
			->Signal(
				m_palette.get_Instrumentation()
					.get_pFence(),
				++m_palette.get_Instrumentation()
					.get_Count());

	D3D12_SHADER_RESOURCE_VIEW_DESC dvShaderResource{
#ifndef NDEBUG
		DXGI_FORMAT_R8G8B8A8_UNORM,
#else
		DXGI_FORMAT_A8_UNORM,
#endif // !NDEBUG
		D3D12_SRV_DIMENSION_TEXTURE2D,
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING
	};
	dvShaderResource.Texture2D.MipLevels = 1U;
	m_palette.get_Instrumentation()
		.get_pD3dDevice()
			->CreateShaderResourceView(
				m_palette.get_Instrumentation()
					.get_pTexture(),
				&dvShaderResource,
				m_palette.get_Instrumentation()
					.get_pvShaderResourceHeap()
						->GetCPUDescriptorHandleForHeapStart());

	if (m_palette.get_Instrumentation()
		.get_pFence()
			->GetCompletedValue() < m_palette.get_Instrumentation()
				.get_Count()) {
		::Windows::ThrowIfFailed(m_palette.get_Instrumentation()
			.get_pFence()
				->SetEventOnCompletion(
					m_palette.get_Instrumentation()
						.get_Count(),
					m_palette.get_Instrumentation()
						.get_hEvent()
		), "Event signaling failed");
		::Windows::wWaitForSingleObject(
			m_palette.get_Instrumentation()
				.get_hEvent(),
			INFINITE
		);
	}
}
void Atlas::Fill(
	const ::std::valarray<double>& point,
	const ::std::valarray<double>& vertex,
	const ::std::valarray<double>& maskPoint,
	const ::std::valarray<double>& color,
	bool blend) {
	using RGBA = ::Voicemeeter::UI::Cherry::Graphics::Theme::RGBA;

	constexpr double AAEPS{ 0.5 - ::std::numeric_limits<double>::epsilon() };

	const size_t& frame{
		m_palette.get_Instrumentation()
			.get_Frame()
	};
	D3D12_VIEWPORT viewport{
		static_cast<FLOAT>(point[0]),
		static_cast<FLOAT>(point[1]),
		static_cast<FLOAT>(vertex[0] + AAEPS),
		static_cast<FLOAT>(vertex[1] + AAEPS)
	};
	m_palette.get_Instrumentation()
		.get_pCommandList(frame)
			->RSSetViewports(1U, &viewport);
	D3D12_RECT scissor{
		static_cast<LONG>(::std::floor(point[0])),
		static_cast<LONG>(::std::floor(point[1])),
		static_cast<LONG>(::std::ceil(point[0] + vertex[0] + AAEPS)),
		static_cast<LONG>(::std::ceil(point[1] + vertex[1] + AAEPS))
	};
	m_palette.get_Instrumentation()
		.get_pCommandList(frame)
			->RSSetScissorRects(1U, &scissor);
	::std::array<FLOAT, 8> cConstants{
		static_cast<FLOAT>(color[RGBA::red]),
		static_cast<FLOAT>(color[RGBA::green]),
		static_cast<FLOAT>(color[RGBA::blue]),
		static_cast<FLOAT>(color[RGBA::alpha]),
		static_cast<FLOAT>(maskPoint[0] / m_pAtlas->get_Width()),
		static_cast<FLOAT>(maskPoint[1] / m_pAtlas->get_Height()),
		static_cast<FLOAT>((vertex[0] + AAEPS) / m_pAtlas->get_Width()),
		static_cast<FLOAT>((vertex[1] + AAEPS) / m_pAtlas->get_Height())
	};
	m_palette.get_Instrumentation()
		.get_pCommandList(frame)
			->SetGraphicsRoot32BitConstants(
				1U,
				static_cast<UINT>(cConstants.size()), cConstants.data(),
				0U);
	m_palette.get_Instrumentation()
		.get_pCommandList(frame)
			->DrawInstanced(4U, 1U, 0U, 0U);
}

Instrumentation::Instrumentation(
	HWND hWnd,
	HMODULE hModule
) : m_pD3dDevice{ nullptr }
  , m_pSwapChain{ nullptr }
  , m_pCommandQueue{ nullptr }
  , m_pvRenderTargetHeap{ nullptr }
  , m_pCompositionTarget{ nullptr }

  , m_frame{ 0 }

  , m_chvRenderTarget{}
  , m_cpRenderTarget{ nullptr, nullptr }
  , m_cpCommandAllocator{ nullptr, nullptr }
  , m_cpCommandList{ nullptr, nullptr }
  , m_cpFence{ nullptr, nullptr }
  , m_chEvent{ NULL, NULL }
  , m_cCount{ 0ULL, 0ULL }

  , m_pRootSignature{ nullptr }
  , m_pPipelineState{ nullptr }
  , m_pCommandAllocator{ nullptr }
  , m_pCommandList{ nullptr }
  , m_pFence{ nullptr }
  , m_hEvent{ NULL }
  , m_count{ 0ULL }
  , m_pVertexBuffer{ nullptr }
  , m_vVertexBuffer{}
  , m_pvShaderResourceHeap{ nullptr }
  , m_pTexture{ nullptr } {
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
		DXGI_GPU_PREFERENCE_UNSPECIFIED,
		IID_PPV_ARGS(&pDxgiAdapter)
	), "Could not get DXGI adapter");

	::Windows::ThrowIfFailed(D3D12CreateDevice(
		pDxgiAdapter.Get(),
		D3D_FEATURE_LEVEL_12_1,
		IID_PPV_ARGS(&m_pD3dDevice)
	), "D3D12 device creation failed");

	D3D12_COMMAND_QUEUE_DESC queueDesc{
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		D3D12_COMMAND_QUEUE_PRIORITY_HIGH,
		D3D12_COMMAND_QUEUE_FLAG_NONE,
		0U
	};
	::Windows::ThrowIfFailed(m_pD3dDevice->CreateCommandQueue(
		&queueDesc,
		IID_PPV_ARGS(&m_pCommandQueue)
	), "Command queue creation failed");

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
		8U, 8U,
		DXGI_FORMAT_R8G8B8A8_UNORM,
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

	D3D12_DESCRIPTOR_HEAP_DESC dRenderTargetView{
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV, FRAME_COUNT,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0U
	};
	::Windows::ThrowIfFailed(m_pD3dDevice->CreateDescriptorHeap(
		&dRenderTargetView,
		IID_PPV_ARGS(&m_pvRenderTargetHeap)
	), "RTV heap descriptor creation failed");

	UINT rtvDescriptorSize{ m_pD3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) };
	D3D12_CPU_DESCRIPTOR_HANDLE hRtv{ m_pvRenderTargetHeap->GetCPUDescriptorHandleForHeapStart() };

	::std::vector<HANDLE> chEvent{ m_hEvent = ::Windows::wCreateEventW(nullptr, FALSE, FALSE, nullptr) };
	auto guardHandle = ::estd::make_guard([&chEvent]()->void {
		for (HANDLE& hEvent : chEvent) {
			CloseHandle(hEvent);
		}
	});
	for (size_t frame{ 0 }; frame < FRAME_COUNT; ++frame) {
		m_chvRenderTarget[frame].ptr = SIZE_T(INT64(hRtv.ptr) + INT64(frame * rtvDescriptorSize));
		::Windows::ThrowIfFailed(m_pSwapChain->GetBuffer(
			static_cast<UINT>(frame),
			IID_PPV_ARGS(&m_cpRenderTarget[frame])
		), "Failed to get swap chain buffer");
		m_pD3dDevice->CreateRenderTargetView(m_cpRenderTarget[frame].Get(), nullptr, m_chvRenderTarget[frame]);

		::Windows::ThrowIfFailed(m_pD3dDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_cpCommandAllocator[frame])
		), "Command allocator creation failed");

		::Windows::ThrowIfFailed(m_pD3dDevice->CreateCommandList1(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT,
			D3D12_COMMAND_LIST_FLAG_NONE,
			IID_PPV_ARGS(&m_cpCommandList[frame])
		), "Command list creation failed");

		::Windows::ThrowIfFailed(m_pD3dDevice->CreateFence(
			m_cCount[frame],
			D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(&m_cpFence[frame])
		), "Fence creation failed");

		chEvent.push_back(m_chEvent[frame] = ::Windows::wCreateEventW(nullptr, FALSE, FALSE, nullptr));
	}
	chEvent.clear();

	::Windows::ThrowIfFailed(m_pD3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_pCommandAllocator)
	), "Command allocator creation failed");

	::Windows::ThrowIfFailed(m_pD3dDevice->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pCommandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&m_pCommandList)
	), "Command list creation failed");

	::Windows::ThrowIfFailed(m_pD3dDevice->CreateFence(
		m_count,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&m_pFence)
	), "Fence creation failed");

	::std::array<FLOAT, 2 * 4> cVertex{
		-1.F, -1.F,
		-1.F, 1.F,
		1.F, -1.F,
		1.F, 1.F
	};
	D3D12_HEAP_PROPERTIES pHeap{
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0U, 0U
	};
	D3D12_RESOURCE_DESC dResource{
		D3D12_RESOURCE_DIMENSION_BUFFER,
		0ULL, sizeof(cVertex), 1U, 1U, 1U,
		DXGI_FORMAT_UNKNOWN,
		DXGI_SAMPLE_DESC{
			1U, 0U
		},
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		D3D12_RESOURCE_FLAG_NONE
	};
	::Windows::ThrowIfFailed(m_pD3dDevice->CreateCommittedResource(
		&pHeap, D3D12_HEAP_FLAG_NONE, &dResource,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_pVertexBuffer)
	), "Vertex buffer creation failed");

	pHeap.Type = D3D12_HEAP_TYPE_UPLOAD;
	::Microsoft::WRL::ComPtr<ID3D12Resource> pUploadBuffer{ nullptr };
	::Windows::ThrowIfFailed(m_pD3dDevice->CreateCommittedResource(
		&pHeap, D3D12_HEAP_FLAG_NONE, &dResource,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&pUploadBuffer)
	), "Upload buffer creation failed");

	D3D12_RANGE range{
		0ULL, 0ULL
	};
	void* pData{ nullptr };
	::Windows::ThrowIfFailed(pUploadBuffer->Map(
		0U, &range,
		&pData
	), "Upload buffer map failed");
	memcpy(pData, cVertex.data(), sizeof(cVertex));
	pUploadBuffer->Unmap(0U, nullptr);

	m_pCommandList->CopyResource(m_pVertexBuffer.Get(), pUploadBuffer.Get());
	D3D12_RESOURCE_BARRIER barrier{
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_TRANSITION_BARRIER{
			m_pVertexBuffer.Get(),
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
		}
	};
	m_pCommandList->ResourceBarrier(1U, &barrier);

	::Windows::ThrowIfFailed(m_pCommandList
		->Close(
	), "Command list close failed");
	::std::array<ID3D12CommandList*, 1> cpCommandList{
		m_pCommandList.Get()
	};
	m_pCommandQueue->ExecuteCommandLists(
		static_cast<UINT>(cpCommandList.size()),
		cpCommandList.data());
	m_pCommandQueue->Signal(m_pFence.Get(), ++m_count);

	m_vVertexBuffer.BufferLocation = m_pVertexBuffer->GetGPUVirtualAddress();
	m_vVertexBuffer.StrideInBytes = 2 * sizeof(FLOAT);
	m_vVertexBuffer.SizeInBytes = sizeof(cVertex);

	D3D12_DESCRIPTOR_HEAP_DESC dShaderResourceHeap{
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		1U,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		0U
	};
	::Windows::ThrowIfFailed(m_pD3dDevice->CreateDescriptorHeap(
		&dShaderResourceHeap,
		IID_PPV_ARGS(&m_pvShaderResourceHeap)
	), "SRV heap descriptor creation failed");

	::std::array<D3D12_ROOT_PARAMETER, 2> cParam{};
	D3D12_DESCRIPTOR_RANGE rDescriptor{
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
		1U,
		0U, 0U,
		0U
	};
	cParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	cParam[0].DescriptorTable.NumDescriptorRanges = 1U;
	cParam[0].DescriptorTable.pDescriptorRanges = &rDescriptor;
	cParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	cParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	cParam[1].Constants.ShaderRegister = 0U;
	cParam[1].Constants.Num32BitValues = 8U;
	cParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	D3D12_STATIC_SAMPLER_DESC dSampler{
		D3D12_FILTER_MIN_MAG_MIP_POINT,
		D3D12_TEXTURE_ADDRESS_MODE_BORDER, D3D12_TEXTURE_ADDRESS_MODE_BORDER, D3D12_TEXTURE_ADDRESS_MODE_BORDER,
		0.F, 0U, D3D12_COMPARISON_FUNC_NEVER,
		D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
		0.F, D3D12_FLOAT32_MAX,
		0U, 0U,
		D3D12_SHADER_VISIBILITY_PIXEL
	};
	D3D12_ROOT_SIGNATURE_DESC dRootSignature{
		static_cast<UINT>(cParam.size()), cParam.data(),
		1U, &dSampler,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	};
	::Microsoft::WRL::ComPtr<ID3DBlob> pSignature{ nullptr };
	::Windows::ThrowIfFailed(D3D12SerializeRootSignature(
		&dRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&pSignature, nullptr
	), "Root signature serialization failed");
	::Windows::ThrowIfFailed(m_pD3dDevice->CreateRootSignature(
		0U,
		pSignature->GetBufferPointer(), pSignature->GetBufferSize(),
		IID_PPV_ARGS(&m_pRootSignature)
	), "Root signature creation failed");

	::Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader{ nullptr };
	::Microsoft::WRL::ComPtr<ID3DBlob> pPixelShader{ nullptr };

	HRSRC hrPixel{ ::Windows::wFindResourceW(hModule, MAKEINTRESOURCE(IDR_PIXEL), L"Shader") };
	HRSRC hrVertex{ ::Windows::wFindResourceW(hModule, MAKEINTRESOURCE(IDR_VERTEX), L"Shader") };

	UINT compileFlags{ 0U };

#ifndef NDEBUG
	compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // !NDEBUG

	::Windows::ThrowIfFailed(D3DCompile(
		::Windows::wLockResource(
			::Windows::wLoadResource(hModule, hrPixel)),
		::Windows::wSizeofResource(hModule, hrPixel),
		NULL,
		nullptr, nullptr,
		"Main", "ps_5_0",
		compileFlags, 0U,
		&pPixelShader, nullptr
	), "Pixel shader compilation failed");
	::Windows::ThrowIfFailed(D3DCompile(
		::Windows::wLockResource(
			::Windows::wLoadResource(hModule, hrVertex)),
		::Windows::wSizeofResource(hModule, hrVertex),
		NULL,
		nullptr, nullptr,
		"Main", "vs_5_0",
		compileFlags, 0U,
		&pVertexShader, nullptr
	), "Vertex shader compilation failed");

	D3D12_INPUT_ELEMENT_DESC dInputElement{
		"POSITION", 0U,
		DXGI_FORMAT_R32G32_FLOAT,
		0U, 0U,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0U
	};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC dPipelineState{
		m_pRootSignature.Get(),
		D3D12_SHADER_BYTECODE{
			pVertexShader->GetBufferPointer(),
			pVertexShader->GetBufferSize()
		},
		D3D12_SHADER_BYTECODE{
			pPixelShader->GetBufferPointer(),
			pPixelShader->GetBufferSize()
		},
		D3D12_SHADER_BYTECODE{}, D3D12_SHADER_BYTECODE{}, D3D12_SHADER_BYTECODE{},
		D3D12_STREAM_OUTPUT_DESC{},
		D3D12_BLEND_DESC{
			FALSE, FALSE,
			{
				D3D12_RENDER_TARGET_BLEND_DESC{
					TRUE, FALSE,
					D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
					D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
					D3D12_LOGIC_OP_NOOP,
					D3D12_COLOR_WRITE_ENABLE_ALL
				}
			}
		},
		UINT_MAX,
		D3D12_RASTERIZER_DESC{
			D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE,
			FALSE,
			D3D12_DEFAULT_DEPTH_BIAS, D3D12_DEFAULT_DEPTH_BIAS_CLAMP, D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
			TRUE,
			FALSE, FALSE, 0U,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
		},
		D3D12_DEPTH_STENCIL_DESC{
			FALSE, D3D12_DEPTH_WRITE_MASK_ZERO, D3D12_COMPARISON_FUNC_NEVER,
			FALSE, 0U, 0U,
			D3D12_DEPTH_STENCILOP_DESC{}, D3D12_DEPTH_STENCILOP_DESC{}
		},
		D3D12_INPUT_LAYOUT_DESC{
			&dInputElement, 1U
		},
		D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		1U, { DXGI_FORMAT_R8G8B8A8_UNORM },
		DXGI_FORMAT_UNKNOWN,
		DXGI_SAMPLE_DESC{
			1U, 0U
		},
		0U,
		D3D12_CACHED_PIPELINE_STATE{},
		D3D12_PIPELINE_STATE_FLAG_NONE
	};
	::Windows::ThrowIfFailed(m_pD3dDevice->CreateGraphicsPipelineState(
		&dPipelineState,
		IID_PPV_ARGS(&m_pPipelineState)
	), "Pipeline state creation failed");

	::Microsoft::WRL::ComPtr<IDCompositionDevice> pCompositionDevice{ nullptr };
	::Windows::ThrowIfFailed(DCompositionCreateDevice(
		nullptr,
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

	if (m_pFence->GetCompletedValue() < m_count) {
		::Windows::ThrowIfFailed(m_pFence->SetEventOnCompletion(
			m_count,
			m_hEvent
		), "Event signaling failed");
		::Windows::wWaitForSingleObject(
			m_hEvent,
			INFINITE
		);
	}
}

Instrumentation::~Instrumentation() {
	for (HANDLE& hEvent : m_chEvent) {
		CloseHandle(hEvent);
	}
	CloseHandle(m_hEvent);
}