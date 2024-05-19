#pragma once

#include <memory>
#include <array>

#include <wrl/client.h>
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi")
#include <d3d12.h>
#pragma comment(lib, "d3d12")
#include <d3dx12.h>
#include <dcomp.h>
#pragma comment(lib, "dcomp")

#include "style.h"

using Microsoft::WRL::ComPtr;

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				class DrawingEngine {
				public:
					class Context {
						static constexpr UINT FrameCount{ 2U };

					public:
						Context(const Context&) = delete;
						Context(Context&&) = delete;

						~Context();

						Context& operator=(const Context&) = delete;
						Context& operator=(Context&&) = delete;

						void Render();
						void Update();
						void Resize(UINT w, UINT h);

					private:
						friend class DrawingEngine;

						struct Frame {
							UINT64 Id{ 0U };
							ComPtr<ID3D12CommandAllocator> pAllocator{ nullptr };
							ComPtr<ID3D12GraphicsCommandList5> pList{ nullptr };
							ComPtr<ID3D12Resource2> pRenderTarget{ nullptr };
						};

						ComPtr<ID3D12Device8> m_pDevice;
						ComPtr<ID3D12CommandQueue> m_pQueue;
						ComPtr<ID3D12DescriptorHeap> m_pRtvHeap;
						UINT m_rtvDescSize;
						ComPtr<IDXGISwapChain4> m_pSwapChain;
						ComPtr<IDCompositionTarget> m_pCompTarget;
						std::array<Frame, FrameCount> m_pFrame;
						UINT m_frameIndex;
						UINT64 m_frameId;
						ComPtr<ID3D12Fence1> m_pFence;
						HANDLE m_fenceEvent;

						Context(IDXGIFactory7* pFactory, ID3D12Device8* pDevice, HWND hWnd, IDCompositionVisual* pCompVisual);

						void WaitForFrame(UINT64 id);
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

					std::unique_ptr<Context> Initialize(HWND hWnd, IDCompositionVisual* pCompVisual);

					private:
						Manifest m_manifest;
						ComPtr<IDXGIFactory7> m_pFactory;
						ComPtr<ID3D12Device8> m_pDevice;
				};
			}
		}
	}
}