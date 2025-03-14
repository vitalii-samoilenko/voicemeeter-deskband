#pragma once

#include <array>

#include <d3d12.h>
#include <dcomp.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include "Voicemeeter.UI.Cherry/Graphics/Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace D3D12 {
			namespace Graphics {
				class Instrumentation final {
				public:
					inline explicit Instrumentation(
						HWND hWnd
					) : Instrumentation{ hWnd, NULL } {

					};
					Instrumentation(
						HWND hWnd,
						HMODULE hModule
					);
					Instrumentation() = delete;
					Instrumentation(const Instrumentation&) = delete;
					Instrumentation(Instrumentation&&) = delete;

					~Instrumentation();

					Instrumentation& operator=(const Instrumentation&) = delete;
					Instrumentation& operator=(Instrumentation&&) = delete;

					inline ID3D12Device8* get_pD3dDevice() const {
						return m_pD3dDevice.Get();
					};
					inline IDXGISwapChain4* get_pSwapChain() const {
						return m_pSwapChain.Get();
					};
					inline ID3D12CommandQueue* get_pCommandQueue() const {
						return m_pCommandQueue.Get();
					};

					static constexpr size_t FRAME_COUNT{ 2 };
					inline size_t& get_Frame() {
						return m_frame;
					};

					inline const D3D12_CPU_DESCRIPTOR_HANDLE& get_hvRenderTarget(size_t frame) const {
						return m_chvRenderTarget[frame];
					};
					inline ID3D12Resource* get_pRenderTarget(size_t frame) const {
						return m_cpRenderTarget[frame].Get();
					};
					inline ID3D12Resource** get_ppRenderTarget(size_t frame) {
						return &m_cpRenderTarget[frame];
					};
					inline ID3D12CommandAllocator* get_pCommandAllocator(size_t frame) const {
						return m_cpCommandAllocator[frame].Get();
					};
					inline ID3D12GraphicsCommandList* get_pCommandList(size_t frame) const {
						return m_cpCommandList[frame].Get();
					};
					inline ID3D12Fence* get_pFence(size_t frame) const {
						return m_cpFence[frame].Get();
					};
					inline const HANDLE& get_hEvent(size_t frame) const {
						return m_chEvent[frame];
					};
					inline UINT64& get_Count(size_t frame) {
						return m_cCount[frame];
					};

					inline ID3D12RootSignature* get_pRootSignature() const {
						return m_pRootSignature.Get();
					};
					inline ID3D12PipelineState* get_pPipelineState() const {
						return m_pPipelineState.Get();
					};
					inline ID3D12PipelineState* get_pBlendPipelineState() const {
						return m_pBlendPipelineState.Get();
					};
					inline ID3D12CommandAllocator* get_pCommandAllocator() const {
						return m_pCommandAllocator.Get();
					};
					inline ID3D12GraphicsCommandList* get_pCommandList() const {
						return m_pCommandList.Get();
					};
					inline ID3D12Fence* get_pFence() const {
						return m_pFence.Get();
					};
					inline const HANDLE& get_hEvent() const {
						return m_hEvent;
					};
					inline UINT64& get_Count() {
						return m_count;
					};
					inline const D3D12_VERTEX_BUFFER_VIEW& get_vVertexBuffer() const {
						return m_vVertexBuffer;
					};
					inline ID3D12DescriptorHeap* get_pvShaderResourceHeap() const {
						return m_pvShaderResourceHeap.Get();
					};
					inline ID3D12Resource* get_pTexture() const {
						return m_pTexture.Get();
					};
					inline ID3D12Resource** get_ppTexture() {
						return &m_pTexture;
					};

				private:
					::Microsoft::WRL::ComPtr<ID3D12Device8> m_pD3dDevice;
					::Microsoft::WRL::ComPtr<IDXGISwapChain4> m_pSwapChain;
					::Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_pCommandQueue;
					::Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pvRenderTargetHeap;
					::Microsoft::WRL::ComPtr<IDCompositionTarget> m_pCompositionTarget;

					size_t m_frame;

					::std::array<D3D12_CPU_DESCRIPTOR_HANDLE, FRAME_COUNT> m_chvRenderTarget;
					::std::array<::Microsoft::WRL::ComPtr<ID3D12Resource>, FRAME_COUNT> m_cpRenderTarget;
					::std::array<::Microsoft::WRL::ComPtr<ID3D12CommandAllocator>, FRAME_COUNT> m_cpCommandAllocator;
					::std::array<::Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>, FRAME_COUNT> m_cpCommandList;
					::std::array<::Microsoft::WRL::ComPtr<ID3D12Fence>, FRAME_COUNT> m_cpFence;
					::std::array<HANDLE, FRAME_COUNT> m_chEvent;
					::std::array<UINT64, FRAME_COUNT> m_cCount;

					::Microsoft::WRL::ComPtr<ID3D12RootSignature> m_pRootSignature;
					::Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pPipelineState;
					::Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pBlendPipelineState;
					::Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_pCommandAllocator;
					::Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_pCommandList;
					::Microsoft::WRL::ComPtr<ID3D12Fence> m_pFence;
					HANDLE m_hEvent;
					UINT64 m_count;
					::Microsoft::WRL::ComPtr<ID3D12Resource> m_pVertexBuffer;
					D3D12_VERTEX_BUFFER_VIEW m_vVertexBuffer;
					::Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pvShaderResourceHeap;
					::Microsoft::WRL::ComPtr<ID3D12Resource> m_pTexture;
				};

				using Atlas = Cherry::Graphics::Atlas<Instrumentation>;
				using Bundle = Cherry::Graphics::Bundle<Instrumentation>;
				using Queue = Cherry::Graphics::Queue<Instrumentation>;
				using Palette = Cherry::Graphics::Palette<Instrumentation>;
			}
		}
	}
}