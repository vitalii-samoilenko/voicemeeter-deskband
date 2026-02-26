#ifndef VOICEMEETER_UI_GRAPHICS_SURFACE_HPP
#define VOICEMEETER_UI_GRAPHICS_SURFACE_HPP

#include <array>
#include <vector>

#include "memory.hpp"
#include "wheel.hpp"

#include "Windows/API.hpp"
#include "Windows/COM.hpp"
#include <d3d12.h>
#include <dcomp.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			class Surface final {
			public:
				inline explicit Surface(HWND hWnd)
					: _hWnd{ hWnd }
					, _hEvent{ ::Windows::CreateEventW(NULL, FALSE, FALSE, NULL) }
					, _device{ nullptr }
					, _commandQueue{ nullptr }
					, _slots_current{ 0 }
					, _slots_commandAllocators{}
					, _slots_commandLists{}
					, _slots_fences{}
					, _slots_counts{}
					, _swapChain{ nullptr }
					, _buffers_hRenderTargetHeap{ nullptr }
					, _buffers_renderTargets{}
					, _buffers_hRenderTargets{}
					, _buffers_fence{ nullptr }
					, _buffers_count{ 0 }
					, _buffers_first{ true }
					, _buffers_invalids{}
					, _compositionTarget{ nullptr } {
					bool failed{ true };
					auto guardEvents = ::estd::make_guard([
							&failed,
							&hEvent = _hEvent
						]()->void {
							if (!failed) {
								return;
							}
							::CloseHandle(hEvent);
						});
					::Windows::ThrowIfFailed(::CoInitialize(
						NULL
					), "COM initialization failed");
					::Microsoft::WRL::ComPtr<IDXGIFactory7> factory{ nullptr };
					{
						UINT factoryFlags{ 0 };
#ifndef NDEBUG
						::Microsoft::WRL::ComPtr<ID3D12Debug> debug{ nullptr };
						::Windows::ThrowIfFailed(::D3D12GetDebugInterface(
							IID_PPV_ARGS(&debug)
						), "Could not get debug interface");
						debug->EnableDebugLayer();
						factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
						::Windows::ThrowIfFailed(::CreateDXGIFactory2(
							factoryFlags,
							IID_PPV_ARGS(&factory)
						), "DXGI factory creation failed");
						::Windows::ThrowIfFailed(factory->MakeWindowAssociation(
							_hWnd,
							DXGI_MWA_NO_ALT_ENTER
						), "Failed to disable fullscreen transition");
						::Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter{ nullptr };
						::Windows::ThrowIfFailed(factory->EnumAdapterByGpuPreference(
							0,
							DXGI_GPU_PREFERENCE_UNSPECIFIED,
							IID_PPV_ARGS(&adapter)
						), "Could not get DXGI adapter");
						::Windows::ThrowIfFailed(::D3D12CreateDevice(
							adapter.Get(),
							D3D_FEATURE_LEVEL_12_1,
							IID_PPV_ARGS(&_device)
						), "D3D12 device creation failed");
					}
					{
						D3D12_COMMAND_QUEUE_DESC queueDesc{
							D3D12_COMMAND_LIST_TYPE_DIRECT,
							D3D12_COMMAND_QUEUE_PRIORITY_HIGH,
							D3D12_COMMAND_QUEUE_FLAG_NONE,
							0
						};
						::Windows::ThrowIfFailed(_device->CreateCommandQueue(
							&queueDesc,
							IID_PPV_ARGS(&_commandQueue)
						), "Command queue creation failed");
					}
					{
						for (size_t slot{ 0 }; slot < SlotsSize; ++slot) {
							::Windows::ThrowIfFailed(_device->CreateCommandAllocator(
								D3D12_COMMAND_LIST_TYPE_DIRECT,
								IID_PPV_ARGS(&_slots_commandAllocators[slot])
							), "Command allocator creation failed");
							::Windows::ThrowIfFailed(_device->CreateCommandList1(
								0, D3D12_COMMAND_LIST_TYPE_DIRECT,
								D3D12_COMMAND_LIST_FLAG_NONE,
								IID_PPV_ARGS(&_slots_commandLists[slot])
							), "Command list creation failed");
							::Windows::ThrowIfFailed(_device->CreateFence(
								_slots_counts[slot],
								D3D12_FENCE_FLAG_NONE,
								IID_PPV_ARGS(&_slots_fences[slot])
							), "Fence creation failed");
						}
					}
					{
						DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
							8, 8,
							DXGI_FORMAT_R16G16B16A16_FLOAT,
							FALSE,
							DXGI_SAMPLE_DESC{
								1, 0
							},
							DXGI_USAGE_RENDER_TARGET_OUTPUT, BuffersSize,
							DXGI_SCALING_STRETCH,
							DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
							DXGI_ALPHA_MODE_PREMULTIPLIED,
							0
						};
						::Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain{ nullptr };
						::Windows::ThrowIfFailed(factory->CreateSwapChainForComposition(
							_commandQueue.Get(),
							&swapChainDesc,
							nullptr,
							&swapChain
						), "DXGI swap chain creation failed");
						::Windows::ThrowIfFailed(swapChain.As(
							&_swapChain
						), "Could not get swap chain");
					}
					{
						D3D12_DESCRIPTOR_HEAP_DESC hRenderTargetHeapDesc{
							D3D12_DESCRIPTOR_HEAP_TYPE_RTV, BuffersSize,
							D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
							0
						};
						::Windows::ThrowIfFailed(_device->CreateDescriptorHeap(
							&hRenderTargetHeapDesc,
							IID_PPV_ARGS(&_buffers_hRenderTargetHeap)
						), "RTV heap descriptor creation failed");
						D3D12_CPU_DESCRIPTOR_HANDLE hRenderTarget{
							_buffers_hRenderTargetHeap->GetCPUDescriptorHandleForHeapStart()
						};
						UINT hRenderTargetSize{
							_device->GetDescriptorHandleIncrementSize(
								D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
						};
						for (size_t buffer{ 0 }; buffer < BuffersSize; ++buffer) {
							::Windows::ThrowIfFailed(_swapChain->GetBuffer(
								static_cast<UINT>(buffer),
								IID_PPV_ARGS(&_buffers_renderTargets[buffer])
							), "Failed to get swap chain buffer");
							_buffers_hRenderTargets[buffer].ptr = SIZE_T(INT64(hRenderTarget.ptr)
								+ INT64(buffer * hRenderTargetSize));
							_device->CreateRenderTargetView(
								_buffers_renderTargets[buffer].Get(),
								nullptr, _buffers_hRenderTargets[buffer]);
						}
						::Windows::ThrowIfFailed(_device->CreateFence(
							_buffers_count,
							D3D12_FENCE_FLAG_NONE,
							IID_PPV_ARGS(&_buffers_fence)
						), "Fence creation failed");
					}
					{
						::Microsoft::WRL::ComPtr<IDCompositionDevice> compositionDevice{ nullptr };
						::Windows::ThrowIfFailed(::DCompositionCreateDevice(
							nullptr,
							IID_PPV_ARGS(&compositionDevice)
						), "Composition device creation failed");
						::Windows::ThrowIfFailed(compositionDevice->CreateTargetForHwnd(
							_hWnd, TRUE,
							&_compositionTarget
						), "Composition target creation failed");
						::Microsoft::WRL::ComPtr<IDCompositionVisual> compositionVisual{ nullptr };
						::Windows::ThrowIfFailed(compositionDevice->CreateVisual(
							&compositionVisual
						), "Composition visual creation failed");
						::Windows::ThrowIfFailed(compositionVisual->SetContent(
							_swapChain.Get()
						), "Could not set swap chain content");
						::Windows::ThrowIfFailed(_compositionTarget->SetRoot(
							compositionVisual.Get()
						), "Could not set composition target root");
						::Windows::ThrowIfFailed(compositionDevice->Commit(
						), "Could not commit composition device");
					}
					failed = false;
				};
				Surface() = delete;
				Surface(Surface const &) = delete;
				Surface(Surface &&) = delete;

				inline ~Surface() {
					::CloseHandle(_hEvent);
				};

				Surface & operator=(Surface const &) = delete;
				Surface & operator=(Surface &&) = delete;

				inline void Resize(vector_t const &value) {
					// TODO: use source size
					if (get_buffers_Fence()
							->GetCompletedValue()
						< get_buffers_Count()) {
						::Windows::ThrowIfFailed(get_buffers_Fence()
							->SetEventOnCompletion(
								get_buffers_Count(),
								get_hEvent()
						), "Event signaling failed");
						::Windows::WaitForSingleObject(
							get_hEvent(), INFINITE);
					}
					for (size_t buffer{ 0 }; buffer < BuffersSize; ++buffer) {
						*geta_buffers_RenderTarget(buffer) = nullptr;
					}
					::Windows::ThrowIfFailed(get_SwapChain()
						->ResizeBuffers(
							0,
							static_cast<UINT>(max(pop(ceil(value[0])), 8)),
							static_cast<UINT>(max(pop(ceil(value[1])), 8)),
							DXGI_FORMAT_UNKNOWN,
							0
					), "Swap chain resize failed");
					for (size_t buffer{ 0 }; buffer < BuffersSize; ++buffer) {
						::Windows::ThrowIfFailed(get_SwapChain()
							->GetBuffer(
								static_cast<UINT>(buffer),
								IID_PPV_ARGS(geta_buffers_RenderTarget(buffer))
						), "Failed to get swap chain buffer");
						get_Device()
							->CreateRenderTargetView(
								get_buffers_RenderTarget(buffer),
								nullptr, get_buffers_hRenderTarget(buffer));
					}
					_first = true;
				};
				inline void Prepare(vector_t const &point, vector_t const &vertex) {
					size_t slot{ inc_slots_Current() };
					size_t buffer{ get_buffers_Current() };
					if (get_slots_Fence(slot)
							->GetCompletedValue()
						< get_slots_Count(slot)) {
						::Windows::ThrowIfFailed(get_slots_Fence(slot)
							->SetEventOnCompletion(
								get_slots_Count(slot),
								get_slots_hEvent(slot)
						), "Event signaling failed");
						::Windows::WaitForSingleObject(
							get_slots_hEvent(), INFINITE);
					}
					::Windows::ThrowIfFailed(get_slots_CommandAllocator(slot)
						->Reset(
					), "Command allocator reset failed");
					::Windows::ThrowIfFailed(get_slots_CommandList(slot)
						->Reset(
							get_slots_CommandAllocator(slot),
							nullptr
					), "Command list reset failed");
					D3D12_RESOURCE_BARRIER barrier{
						D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
						D3D12_RESOURCE_BARRIER_FLAG_NONE,
						D3D12_RESOURCE_TRANSITION_BARRIER{
							get_buffers_RenderTarget(buffer),
							D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
							D3D12_RESOURCE_STATE_PRESENT,
							D3D12_RESOURCE_STATE_RENDER_TARGET
						}
					};
					get_slots_CommandList(slot)
						->ResourceBarrier(1U, &barrier);
					D3D12_CPU_DESCRIPTOR_HANDLE hRenderTarget{
						get_buffers_hRenderTarget(buffer)
					};
					get_slots_CommandList(slot)
						->OMSetRenderTargets(
							1, &hRenderTarget, FALSE, nullptr);
					D3D12_RECT const &target{ Invalidate(point, vertex) };
					FLOAT transparent[]{ 0.F, 0.F, 0.F, 0.F };
					get_slots_CommandList(slot)
						->ClearRenderTargetView(
							get_buffers_hRenderTarget(buffer),
							transparent,
							1U, &target);
					::Windows::ThrowIfFailed(get_slots_CommandList(slot)
						->Close(
					), "Command list close failed");
					ID3D12CommandList *commandList{ get_slots_CommandList(slot) };
					get_CommandQueue()
						->ExecuteCommandLists(1, &commandList);
					get_CommandQueue()
						->Signal(
							get_slots_Fence(slot),
							inc_slots_Count(slot));
					get_CommandQueue()
						->Signal(
							get_buffers_Fence(),
							inc_buffers_Count());
				};
				inline void Commit() {
					size_t slot{ inc_slots_Current() };
					size_t buffer{ get_buffers_Current() };
					if (get_slots_Fence(slot)
							->GetCompletedValue()
						< get_slots_Count(slot)) {
						::Windows::ThrowIfFailed(get_slots_Fence(slot)
							->SetEventOnCompletion(
								get_slots_Count(slot),
								get_hEvent()
						), "Event signaling failed");
						::Windows::WaitForSingleObject(
							get_hEvent(), INFINITE);
					}
					::Windows::ThrowIfFailed(get_slots_CommandAllocator(slot)
						->Reset(
					), "Command allocator reset failed");
					::Windows::ThrowIfFailed(get_slots_CommandList(slot)
						->Reset(
							get_slots_CommandAllocator(slot),
							nullptr
					), "Command list reset failed");
					D3D12_RESOURCE_BARRIER barrier{
						D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
						D3D12_RESOURCE_BARRIER_FLAG_NONE,
						D3D12_RESOURCE_TRANSITION_BARRIER{
							get_buffers_RenderTarget(buffer),
							D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
							D3D12_RESOURCE_STATE_RENDER_TARGET,
							D3D12_RESOURCE_STATE_PRESENT
						}
					};
					get_slots_CommandList(slot)
						->ResourceBarrier(1U, &barrier);
					::Windows::ThrowIfFailed(get_slots_CommandList(slot)
						->Close(
					), "Command list close failed");
					ID3D12CommandList *commandList{ get_slots_CommandList(slot) };
					get_CommandQueue()
						->ExecuteCommandLists(1, &commandList);
					get_CommandQueue()
						->Signal(
							get_slots_Fence(slot),
							inc_slots_Count(slot));
					Present();
				}

				static constexpr size_t BuffersSize{ 2 };
				static constexpr size_t SlotsSize{ 3 };

				inline HWND get_hWnd() const {
					return _hWnd;
				};
				inline HANDLE get_hEvent() const {
					return _hEvent;
				};
				inline ID3D12Device8 * get_Device() const {
					return _device.Get();
				};
				inline ID3D12CommandQueue * get_CommandQueue() const {
					return _commandQueue.Get();
				};
				// slots
				inline size_t get_slots_Current() const {
					return _slots_current;
				};
				inline size_t inc_slots_Current() {
					return (_slots_current = (_slots_current + 1) % SlotsSize);
				};
				inline ID3D12CommandAllocator * get_slots_CommandAllocator(size_t slot) const {
					return _slots_commandAllocators[slot].Get();
				};
				inline ID3D12GraphicsCommandList * get_slots_CommandList(size_t slot) const {
					return _slots_commandLists[slot].Get();
				};
				inline ID3D12Fence * get_slots_Fence(size_t slot) const {
					return _slots_fences[slot].Get();
				};
				inline UINT64 get_slots_Count(size_t slot) const {
					return _slots_counts[slot];
				};
				inline UINT64 inc_slots_Count(size_t slot) {
					return ++_slots_counts[slot];
				};
				// -----
				inline IDXGISwapChain4 * get_SwapChain() const {
					return _swapChain.Get();
				};
				// buffers
				inline size_t get_buffers_Current() const {
					return _swapChain->GetCurrentBackBufferIndex();
				};
				inline ID3D12Resource * get_buffers_RenderTarget(size_t buffer) const {
					return _buffers_rengerTargets[buffer].Get();
				};
				inline ID3D12Resource ** geta_buffers_RenderTarget(size_t buffer) {
					return &_buffers_renderTargets[buffer];
				};
				inline D3D12_CPU_DESCRIPTOR_HANDLE get_buffers_hRenderTarget(size_t buffer) const {
					return _buffers_hRenderTargets[buffer];
				};
				inline ID3D12Fence get_buffers_Fence() const {
					return _buffers_fence.Get();
				};
				inline UINT64 get_buffers_Count() const {
					return _buffers_count;
				};
				inline UINT64 inc_buffers_Count() {
					return ++_buffers_count;
				};
				// -----

				inline D3D12_RECT const & Invalidate(vector_t const &point, vector_t const &vertex) {
					_buffers_invalids.emplace_back(
						static_cast<LONG>(pop(floor(point[0]))),
						static_cast<LONG>(pop(floor(point[1]))),
						static_cast<LONG>(pop(ceil(point[0] + vertex[0]))),
						static_cast<LONG>(pop(ceil(point[1] + vertex[1]))));
					return _buffers_invalids.back();
				};
				inline void Present() {
					if (_buffers_first) {
						::Windows::ThrowIfFailed(get_SwapChain()
							->Present(
								0U, 0U
						), "Presentation failed");
						_first = false;
					} else {
						DXGI_PRESENT_PARAMETERS params{
							_buffers_invalids.size(), &_buffers_invalids[0],
							nullptr,
							nullptr
						};
						::Windows::ThrowIfFailed(get_SwapChain()
							->Present1(
								0U, 0U,
								&params
						), "Presentation failed");
					}
					get_CommandQueue()
						->Signal(
							get_buffers_Fence(),
							inc_buffers_Count());
					_buffers_invalids.clear();
				};

			private:
				HWND _hWnd;
				HANDLE _hEvent;
				::Microsoft::WRL::ComPtr<ID3D12Device8> _device;
				::Microsoft::WRL::ComPtr<ID3D12CommandQueue> _commandQueue;
				// slots
				size_t _slots_current;
				::std::array<::Microsoft::WRL::ComPtr<ID3D12CommandAllocator>, SlotsSize> _slots_commandAllocators;
				::std::array<::Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>, SlotsSize> _slots_commandLists;
				::std::array<::Microsoft::WRL::ComPtr<ID3D12Fence>, SlotsSize> _slots_fences;
				::std::array<UINT64, SlotsSize> _slots_counts;
				// -----
				::Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapChain;
				// buffers
				::Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _buffers_hRenderTargetHeap;
				::std::array<::Microsoft::WRL::ComPtr<ID3D12Resource>, BuffersSize> _buffers_renderTargets;
				::std::array<D3D12_CPU_DESCRIPTOR_HANDLE, BuffersSize> _buffers_hRenderTargets;
				::Microsoft::WRL::ComPtr<ID3D12Fence> _buffers_fence;
				UINT64 _buffers_count;
				bool _buffers_first;
				::std::vector<D3D12_RECT> _buffers_invalids;
				// -----
				::Microsoft::WRL::ComPtr<IDCompositionTarget> _compositionTarget;
			};
		}
	}
}

#endif
