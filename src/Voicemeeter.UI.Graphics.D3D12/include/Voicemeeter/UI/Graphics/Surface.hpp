#ifndef VOICEMEETER_UI_GRAPHICS_SURFACE_HPP
#define VOICEMEETER_UI_GRAPHICS_SURFACE_HPP

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
					, _swapChain{ nullptr }
					, _compositionTarget{ nullptr } {
					::Windows::ThrowIfFailed(::CoInitialize(
						NULL
					), "COM initialization failed");
					::Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory{ nullptr };
					{
						UINT dxgiFactoryFlags{ 0 };
#ifndef NDEBUG
						::Microsoft::WRL::ComPtr<ID3D12Debug> d3dDebug{ nullptr };
						::Windows::ThrowIfFailed(::D3D12GetDebugInterface(
							IID_PPV_ARGS(&d3dDebug)
						), "Could not get debug interface");
						d3dDebug->EnableDebugLayer();
						dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
						::Windows::ThrowIfFailed(::CreateDXGIFactory2(
							dxgiFactoryFlags,
							IID_PPV_ARGS(&dxgiFactory)
						), "DXGI factory creation failed");
						::Windows::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(
							_hWnd,
							DXGI_MWA_NO_ALT_ENTER
						), "Failed to disable fullscreen transition");
						::Microsoft::WRL::ComPtr<IDXGIAdapter4> dxgiAdapter{ nullptr };
						::Windows::ThrowIfFailed(dxgiFactory->EnumAdapterByGpuPreference(
							0,
							DXGI_GPU_PREFERENCE_UNSPECIFIED,
							IID_PPV_ARGS(&dxgiAdapter)
						), "Could not get DXGI adapter");
						::Windows::ThrowIfFailed(::D3D12CreateDevice(
							dxgiAdapter.Get(),
							D3D_FEATURE_LEVEL_12_1,
							IID_PPV_ARGS(&_d3dDevice)
						), "D3D12 device creation failed");
					}
					{
						D3D12_COMMAND_QUEUE_DESC queueDesc{
							D3D12_COMMAND_LIST_TYPE_DIRECT,
							D3D12_COMMAND_QUEUE_PRIORITY_HIGH,
							D3D12_COMMAND_QUEUE_FLAG_NONE,
							0
						};
						::Windows::ThrowIfFailed(_d3dDevice->CreateCommandQueue(
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
						::Windows::ThrowIfFailed(dxgiFactory->CreateSwapChainForComposition(
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
				};
				Surface() = delete;
				Surface(Surface const &) = delete;
				Surface(Surface &&) = delete;

				inline ~Surface() = default;

				Surface & operator=(Surface const &) = delete;
				Surface & operator=(Surface &&) = delete;

				inline void set_Size(vector_t const &vertex) {
					// TODO: use source size
					::Windows::ThrowIfFailed(_swapChain->ResizeBuffers(
						0,
						static_cast<UINT>(max(pop(ceil(vertex[0])), 8)),
						static_cast<UINT>(max(pop(ceil(vertex[1])), 8)),
						DXGI_FORMAT_UNKNOWN,
						0
					), "Swap chain resize failed");
					_first = true;
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

			private:
				HWND _hWnd;
				::Microsoft::WRL::ComPtr<ID3D12Device8> _device;
				::Microsoft::WRL::ComPtr<ID3D12CommandQueue> _commandQueue;
				::Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapChain;
				::Microsoft::WRL::ComPtr<IDCompositionTarget> _compositionTarget;
				bool _first;
			};
		}
	}
}

#endif
