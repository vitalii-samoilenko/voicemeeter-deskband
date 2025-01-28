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
					explicit Instrumentation(
						HWND hWnd
					);
					Instrumentation() = delete;
					Instrumentation(const Instrumentation&) = delete;
					Instrumentation(Instrumentation&&) = delete;

					~Instrumentation();

					Instrumentation& operator=(const Instrumentation&) = delete;
					Instrumentation& operator=(Instrumentation&&) = delete;

					inline ID3D12Device10* get_pD3dDevice() const {
						return m_pD3dDevice.Get();
					};
					inline IDXGISwapChain1* get_pSwapChain() const {
						return m_pSwapChain.Get();
					};
					inline ID3D12DescriptorHeap* get_pRtvHeap() const {
						return m_pRtvHeap.Get();
					};

					inline const D3D12_CPU_DESCRIPTOR_HANDLE& get_hRtv() const {
						return m_chRtv[m_frame];
					};
					inline ID3D12Resource* get_pRenderTarget() const {
						return m_cpRenderTarget[m_frame].Get();
					};
					inline ID3D12CommandAllocator* get_pCommandAllocator() const {
						return m_cpCommandAllocator[m_frame].Get();
					};
					inline ID3D12GraphicsCommandList* get_pCommandList() const {
						return m_cpCommandList[m_frame].Get();
					};
					inline ID3D12Fence* get_pFence() const {
						return m_cpFence[m_frame].Get();
					};
					inline const HANDLE& get_hEvent() const {
						return m_chEvent[m_frame];
					};

					inline UINT64 Tick() {
						return ++m_cCount[m_frame];
					};

				private:
					::Microsoft::WRL::ComPtr<ID3D12Device10> m_pD3dDevice;
					::Microsoft::WRL::ComPtr<IDXGISwapChain4> m_pSwapChain;
					::Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_pCommandQueue;
					::Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pRtvHeap;
					::Microsoft::WRL::ComPtr<IDCompositionTarget> m_pCompositionTarget;

					static constexpr size_t FRAME_COUNT{ 2 };
					size_t m_frame;

					::std::array<D3D12_CPU_DESCRIPTOR_HANDLE, FRAME_COUNT> m_chRtv;
					::std::array<::Microsoft::WRL::ComPtr<ID3D12Resource>, FRAME_COUNT> m_cpRenderTarget;
					::std::array<::Microsoft::WRL::ComPtr<ID3D12CommandAllocator>, FRAME_COUNT> m_cpCommandAllocator;
					::std::array<::Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>, FRAME_COUNT> m_cpCommandList;
					::std::array<::Microsoft::WRL::ComPtr<ID3D12Fence>, FRAME_COUNT> m_cpFence;
					::std::array<HANDLE, FRAME_COUNT> m_chEvent;
					::std::array<UINT64, FRAME_COUNT> m_cCount;
				};

				using Atlas = Cherry::Graphics::Atlas<Instrumentation>;
				using Bundle = Cherry::Graphics::Bundle<Instrumentation>;
				using Queue = Cherry::Graphics::Queue<Instrumentation>;
				using Palette = Cherry::Graphics::Palette<Instrumentation>;
			}
		}
	}
}