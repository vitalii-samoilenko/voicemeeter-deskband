#ifndef VOICEMEETER_UI_GRAPHICS_SURFACE_HPP
#define VOICEMEETER_UI_GRAPHICS_SURFACE_HPP

#include <array>
#include <memory>
#include <unordered_map>

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
					, _device{ nullptr }
					, _commandQueue{ nullptr }
					, _commandAllocator{ nullptr }
					, _commandList{ nullptr }
					, _fence{ nullptr }
					, _hEvent{ NULL }
					, _count{ 0 }
					, _swapChain{ nullptr }
					, _hRenderTargetHeap{ nullptr }
					, _renderTargets{}
					, _hRenderTargets{}
					, _compositionTarget{ nullptr }
					, _first{ false }
					, _binders{} {
					bool failed{ true };
					auto guardEvent = ::estd::make_guard([
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
						DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
							8, 8,
							DXGI_FORMAT_R8G8B8A8_UNORM,
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
							IID_PPV_ARGS(&_hRenderTargetHeap)
						), "RTV heap descriptor creation failed");
						D3D12_CPU_DESCRIPTOR_HANDLE hRenderTarget{
							_hRenderTargetHeap->GetCPUDescriptorHandleForHeapStart()
						};
						UINT hRenderTargetSize{
							_device->GetDescriptorHandleIncrementSize(
								D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
						};
						for (size_t buffer{ 0 }; buffer < BuffersSize; ++buffer) {
							::Windows::ThrowIfFailed(_swapChain->GetBuffer(
								static_cast<UINT>(buffer),
								IID_PPV_ARGS(&_renderTargets[buffer])
							), "Failed to get swap chain buffer");
							_hRenderTargets[buffer].ptr = SIZE_T(INT64(hRenderTarget.ptr)
								+ INT64(buffer * hRenderTargetSize));
							_device->CreateRenderTargetView(
								_renderTargets[buffer].Get(),
								nullptr, _hRenderTargets[buffer]);
						}
					}
					{
						::Windows::ThrowIfFailed(_device->CreateCommandAllocator(
							D3D12_COMMAND_LIST_TYPE_DIRECT,
							IID_PPV_ARGS(&_commandAllocator)
						), "Command allocator creation failed");
						::Windows::ThrowIfFailed(_device->CreateCommandList1(
							0, D3D12_COMMAND_LIST_TYPE_DIRECT,
							D3D12_COMMAND_LIST_FLAG_NONE,
							IID_PPV_ARGS(&_commandList)
						), "Command list creation failed");
						_hEvent = ::Windows::CreateEventW(NULL, FALSE, FALSE, NULL);
						::Windows::ThrowIfFailed(_device->CreateFence(
							_count,
							D3D12_FENCE_FLAG_NONE,
							IID_PPV_ARGS(&_fence)
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

				inline void set_Size(vector_t const &value) {
					// TODO: use source size
					for (auto &[clientId, binder] : _binders) {
						binder->Unbind(*this);
					}
					if (_fence->GetCompletedValue() < _count) {
						::Windows::ThrowIfFailed(_fence->SetEventOnCompletion(
							_count,
							_hEvent
						), "Event signaling failed");
						::Windows::WaitForSingleObject(
							_hEvent, INFINITE);
					}
					for (size_t buffer{ 0 }; buffer < BuffersSize; ++buffer) {
						_renderTargets[buffer] = nullptr;
					}
					::Windows::ThrowIfFailed(_swapChain->ResizeBuffers(
						0,
						static_cast<UINT>(max(pop(ceil(value[0])), 8)),
						static_cast<UINT>(max(pop(ceil(value[1])), 8)),
						DXGI_FORMAT_UNKNOWN,
						0
					), "Swap chain resize failed");
					for (size_t buffer{ 0 }; buffer < BuffersSize; ++buffer) {
						::Windows::ThrowIfFailed(_swapChain->GetBuffer(
							static_cast<UINT>(buffer),
							IID_PPV_ARGS(&_renderTargets[buffer])
						), "Failed to get swap chain buffer");
						_device->CreateRenderTargetView(
							_renderTargets[buffer].Get(),
							nullptr, _hRenderTargets[buffer]);
					}
					for (auto &[clientId, binder] : _binders) {
						binder->Bind(*this);
					}
					_first = true;
				};

				inline void Clear(vector_t const &point, vector_t const &vertex) {
					size_t buffer{ _swapChain->GetCurrentBackBufferIndex() };
					if (_fence->GetCompletedValue() < _count) {
						::Windows::ThrowIfFailed(_fence->SetEventOnCompletion(
							_count,
							_hEvent
						), "Event signaling failed");
						::Windows::WaitForSingleObject(
							_hEvent, INFINITE);
					}
					::Windows::ThrowIfFailed(_commandAllocator->Reset(
					), "Command allocator reset failed");
					::Windows::ThrowIfFailed(_commandList->Reset(
						_commandAllocator.Get(),
						nullptr
					), "Command list reset failed");
					_commandList->OMSetRenderTargets(
						1, &_hRenderTargets[buffer], FALSE, nullptr);
					D3D12_RECT rect{
						static_cast<LONG>(pop(floor(point[0]))),
						static_cast<LONG>(pop(floor(point[1]))),
						static_cast<LONG>(pop(ceil(point[0] + vertex[0]))),
						static_cast<LONG>(pop(ceil(point[1] + vertex[1])))
					};
					FLOAT transparent[]{ 0.F, 0.F, 0.F, 0.F };
					_commandList->ClearRenderTargetView(
						_hRenderTargets[buffer],
						transparent,
						1U, &rect);
					::Windows::ThrowIfFailed(_commandList->Close(
					), "Command list close failed");
					ID3D12CommandList *commandList{ _commandList.Get() };
					_commandQueue->ExecuteCommandLists(
						1, &commandList);
					_commandQueue->Signal(
						_fence.Get(), ++_count);
				};

				inline void Present(vector_t const &point, vector_t const &vertex) {
					if (_first) {
						::Windows::ThrowIfFailed(_swapChain->Present(
							0U, 0U
						), "Presentation failed");
						_first = false;
					} else {
						RECT rect{
							static_cast<LONG>(pop(floor(point[0]))),
							static_cast<LONG>(pop(floor(point[1]))),
							static_cast<LONG>(pop(ceil(point[0] + vertex[0]))),
							static_cast<LONG>(pop(ceil(point[1] + vertex[1])))
						};
						DXGI_PRESENT_PARAMETERS params{
							1, &rect,
							nullptr,
							nullptr
						};
						::Windows::ThrowIfFailed(_swapChain->Present1(
							0U, 0U,
							&params
						), "Presentation failed");
					}
				}

				class token final {
				public:
					token() = delete;
					token(token const &) = delete;
					inline token(token &&other)
						: that{ other.that }
						, _clientId{ other._clientId } {
						other._clientId = nullptr;
					};

					inline ~token() {
						if (!_clientId) {
							return;
						}
						that->_binders
							.erase(_clientId);
					};

					token & operator=(token const &) = delete;
					token & operator=(token &&) = delete;

					template<typename TBinder>
					inline void on_reallocate(TBinder &target) {
						that->_binders[_clientId]
							= ::std::make_unique<
								Binder<TBinder>>(
								&target);
					};

				private:
					friend class Surface;

					inline token(
						Surface *that,
						void const *clientId)
						: that{ that }
						, _clientId{ clientId } {

					};

					Surface *that;
					void const *_clientId;
				};
				template<typename TClient>
				inline token Subscribe() {
					return token{ this, &typeid(TClient) };
				};

				static constexpr size_t BuffersSize{ 2 };

				inline HWND get_hWnd() const {
					return _hWnd;
				};
				inline ID3D12Device8 * get_Device() const {
					return _device.Get();
				};
				inline ID3D12CommandQueue * get_CommandQueue() const {
					return _commandQueue.Get();
				};
				inline IDXGISwapChain4 * get_SwapChain() const {
					return _swapChain.Get();
				};
				inline ID3D12Resource * get_RenderTarget(size_t buffer) const {
					return _renderTargets[buffer].Get();
				};
				inline D3D12_CPU_DESCRIPTOR_HANDLE get_hRenderTarget(size_t buffer) const {
					return _hRenderTargets[buffer];
				};

			private:
				class IBinder {
				public:
					IBinder(IBinder const &) = delete;
					IBinder(IBinder &&) = delete;

					virtual ~IBinder() {};

					IBinder & operator=(IBinder const &) = delete;
					IBinder & operator=(IBinder &&) = delete;

					virtual void Bind(Surface &target) = 0;
					virtual void Unbind(Surface &target) = 0;

				protected:
					inline IBinder() = default;
				};
				template<typename TBinder>
				class Binder final : public IBinder {
				public:
					inline explicit Binder(TBinder *that)
						: that{ that } {

					};
					Binder() = delete;
					Binder(Binder const &) = delete;
					Binder(Binder &&) = delete;

					inline ~Binder() = default;

					Binder & operator=(Binder const &) = delete;
					Binder & operator=(Binder &&) = delete;

					virtual void Bind(Surface &target) override {
						that->Bind(target);
					};
					virtual void Unbind(Surface &target) override {
						that->Unbind(target);
					};

				private:
					TBinder *that;
				};

				friend class token;

				HWND _hWnd;
				::Microsoft::WRL::ComPtr<ID3D12Device8> _device;
				::Microsoft::WRL::ComPtr<ID3D12CommandQueue> _commandQueue;
				::Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _commandAllocator;
				::Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _commandList;
				::Microsoft::WRL::ComPtr<ID3D12Fence> _fence;
				HANDLE _hEvent;
				UINT64 _count;
				::Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapChain;
				::Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _hRenderTargetHeap;
				::std::array<::Microsoft::WRL::ComPtr<ID3D12Resource>, BuffersSize> _renderTargets;
				::std::array<D3D12_CPU_DESCRIPTOR_HANDLE, BuffersSize> _hRenderTargets;
				::Microsoft::WRL::ComPtr<IDCompositionTarget> _compositionTarget;
				bool _first;
				::std::unordered_map<
					void const *,
					::std::unique_ptr<IBinder>
				> _binders;
			};
		}
	}
}

#endif
