#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include <cmath>
#include <vector>

#include "estd/memory.h"

#include <d3dcompiler.h>

#include "../Shaders.h"
#include "Windows/Wrappers.h"

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

	m_pAtlas.reset(new ::Voicemeeter::Atlas::Cherry{ NULL });

	BYTE* pSrc{ reinterpret_cast<BYTE*>(m_pAtlas->get_pField()) };

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
		DXGI_FORMAT_R32_FLOAT,
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

	UINT rowSize{ m_pAtlas->get_Width() * 4 };
	UINT srcRowPitch{ rowSize };
	UINT remainder{ rowSize % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT };
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
				DXGI_FORMAT_R32_FLOAT,
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
		DXGI_FORMAT_R32_FLOAT,
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
