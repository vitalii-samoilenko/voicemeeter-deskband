#pragma once

#include<memory>

#include <wrl/client.h>
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi")
#include <d3d12.h>
#pragma comment(lib, "d3d12")
#include <d3dx12.h>

#include "style.h"

using Microsoft::WRL::ComPtr;

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				class DrawingEngine {
					static constexpr UINT FrameCount{ 2U };

				public:
					class Context {
					public:
						Context(const Context&) = delete;
						Context(Context&&) = delete;

						~Context();

						Context& operator=(const Context&) = delete;
						Context& operator=(Context&&) = delete;

						void Render();
						void Update() const;
						void Resize(UINT w, UINT h);

					private:
						friend class DrawingEngine;

						ComPtr<ID3D12Device8> m_pDevice;
						ComPtr<ID3D12CommandQueue> m_pQueue;
						ComPtr<ID3D12CommandAllocator> m_pAllocator;
						ComPtr<ID3D12GraphicsCommandList5> m_pList;
						ComPtr<ID3D12Fence1> m_pFence;
						HANDLE m_fenceEvent;
						UINT64 m_fenceValue;
						ComPtr<ID3D12DescriptorHeap> m_pRtvHeap;
						ComPtr<IDXGISwapChain4> m_pSwapChain;
						UINT m_rtvDescSize;
						ComPtr<ID3D12Resource2> m_pRenderTargets[FrameCount];
						UINT m_frame;

						inline Context() noexcept
							: m_pDevice{ nullptr }
							, m_pQueue{ nullptr }
							, m_pAllocator{ nullptr }
							, m_pList{ nullptr }
							, m_pFence{ nullptr }
							, m_fenceEvent{ NULL }
							, m_fenceValue{ 0U }
							, m_pRtvHeap{ nullptr }
							, m_pSwapChain{ nullptr }
							, m_rtvDescSize{ 0U }
							, m_pRenderTargets{ nullptr, nullptr }
							, m_frame{ 0U } {

						};

						void WaitForGpu();
					};
					class Manifest {
					public:
						Manifest(const Manifest&) = delete;
						Manifest(Manifest&&) = delete;

						~Manifest() = default;

						Manifest& operator=(const Manifest&) = delete;
						Manifest& operator=(Manifest&&) = delete;

					private:
						friend class DrawingEngine;

						inline Manifest() noexcept {

						}
					};

					inline const Manifest& get_manifest() const noexcept {
						return m_manifest;
					}

					explicit DrawingEngine(const Style& style);
					DrawingEngine() = delete;
					DrawingEngine(const DrawingEngine&) = delete;
					DrawingEngine(DrawingEngine&&) = delete;

					~DrawingEngine() = default;

					DrawingEngine& operator=(const DrawingEngine&) = delete;
					DrawingEngine& operator=(DrawingEngine&&) = delete;

					std::unique_ptr<Context> Initialize(HWND hWnd);

					private:
						Manifest m_manifest;
						ComPtr<IDXGIFactory7> m_pFactory;
						ComPtr<ID3D12Device8> m_pDevice;
				};
			}
		}
	}
}