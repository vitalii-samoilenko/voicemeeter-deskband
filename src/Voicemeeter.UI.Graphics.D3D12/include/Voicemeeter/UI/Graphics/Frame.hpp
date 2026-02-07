#ifndef VOICEMEETER_UI_GRAPHICS_FRAME_HPP
#define VOICEMEETER_UI_GRAPHICS_FRAME_HPP

#include <utility>

#include "wheel.hpp"

#include "Windows/API.hpp"
#include <d3d12.h>

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			template<
				typename TState,
				typename TQueue,
				typename TStopwatch>
			class Frame final {
			public:
				inline Frame(
					TState &state,
					TQueue &queue,
					TStopwatch &stopwatch)
					: _state{ state }
					, _queue{ queue }
					, _stopwatch{ stopwatch }
					, _first{ true } {

				};
				Frame() = delete;
				Frame(Frame const &) = delete;
				Frame(Frame &&) = delete;

				inline ~Frame() = default;

				Frame & operator=(Frame const &) = delete;
				Frame & operator=(Frame &&) = delete;

				inline void set_Size(vector_t const &vertex) {
					// TODO: use source size
					for (size_t frame{ 0 }; frame < TState::FrameCount; ++frame) {
						if (_state.get_Fence(frame)
							->GetCompletedValue() < _state.get_Count(frame)) {
							::Windows::ThrowIfFailed(_state.get_Fence(frame)
								->SetEventOnCompletion(
									_state.get_Count(frame),
									_state.get_hEvent(frame)
							), "Event signaling failed");
							::Windows::WaitForSingleObject(
								_state.get_hEvent(frame), INFINITE);
						}
						*_state.geta_RenderTarget(frame) = nullptr;
					}
					::Windows::ThrowIfFailed(_state.get_SwapChain()
						->ResizeBuffers(
							0,
							static_cast<UINT>(ceil(vertex[0])),
							static_cast<UINT>(ceil(vertex[1])),
							DXGI_FORMAT_UNKNOWN,
							0
					), "Swap chain resize failed");
					for (size_t frame{ 0 }; frame < TState::FrameCount; ++frame) {
						::Windows::ThrowIfFailed(_state.get_SwapChain()
							->GetBuffer(
								static_cast<UINT>(frame),
								IID_PPV_ARGS(_state.geta_RenderTarget(frame))
						), "Failed to get swap chain buffer");
						_state.get_D3dDevice()
							->CreateRenderTargetView(
								_state.get_RenderTarget(frame),
								nullptr,
								_state.get_hRenderTarget(frame));
					}
					_first = true;
				};

				inline void Update() {
					// TODO: render to images
					_stopwatch.Lap();
					if (_queue.empty()) {
						return;
					}
					TQueue queue{ ::std::move(_queue) };
					size_t frame{
						_state.get_SwapChain()
							->GetCurrentBackBufferIndex()
					};
					if (_state.get_Fence(frame)
							->GetCompletedValue() < _state.get_Count(frame)) {
						::Windows::ThrowIfFailed(_state.get_Fence(frame)
							->SetEventOnCompletion(
								_state.get_Count(frame),
								_state.get_hEvent(frame)
						), "Event signaling failed");
						::Windows::WaitForSingleObject(
							_state.get_hEvent(frame), INFINITE);
					}
					::Windows::ThrowIfFailed(_state.get_CommandAllocator(frame)
						->Reset(
					), "Command allocator reset failed");
					::Windows::ThrowIfFailed(_state.get_CommandList(frame)
						->Reset(
							_state.get_CommandAllocator(frame),
							_state.get_DefaultState()
					), "Command list reset failed");
					_state.get_CommandList(frame)
						->SetGraphicsRootSignature(
							_state.get_RootSignature());
					ID3D12DescriptorHeap *hTextureHeap{ _state.get_hTextureHeap() };
					_state.get_CommandList(frame)
						->SetDescriptorHeaps(1, &hTextureHeap);
					_state.get_CommandList(frame)
						->SetGraphicsRootDescriptorTable(
							0,
							_state.get_hTextureHeap()
								->GetGPUDescriptorHandleForHeapStart());
					_state.get_CommandList(frame)
						->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
					D3D12_VERTEX_BUFFER_VIEW hSquareBuffer{ _state.get_hSquareBuffer() };
					_state.get_CommandList(frame)
						->IASetVertexBuffers(0, 1, &hSquareBuffer);
					D3D12_RESOURCE_BARRIER barrier{
						D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
						D3D12_RESOURCE_BARRIER_FLAG_NONE,
						D3D12_RESOURCE_TRANSITION_BARRIER{
							_state.get_RenderTarget(frame),
							D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
							D3D12_RESOURCE_STATE_PRESENT,
							D3D12_RESOURCE_STATE_RENDER_TARGET
						}
					};
					_state.get_CommandList(frame)
						->ResourceBarrier(1, &barrier);
					D3D12_CPU_DESCRIPTOR_HANDLE hRenderTarget{ _state.get_hRenderTarget(frame) };
					_state.get_CommandList(frame)
						->OMSetRenderTargets(1, &hRenderTarget, FALSE, nullptr);
					for (auto &bundle : queue) {
						bundle->operator()();
					}
					barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
					barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
					_state.get_CommandList(frame)
						->ResourceBarrier(1, &barrier);
					::Windows::ThrowIfFailed(_state.get_CommandList(frame)
						->Close(
					), "Command list close failed");
					ID3D12CommandList *commandList{ _state.get_CommandList(frame) };
					_state.get_CommandQueue()
						->ExecuteCommandLists(1, &commandList);
					_state.get_CommandQueue()
						->Signal(
							_state.get_Fence(frame),
							_state.inc_Count(frame));
				};
				inline void Present(vector_t const &point, vector_t const &vertex) {
					// TODO: bitblt
					if (_first) {
						::Windows::ThrowIfFailed(_state.get_SwapChain()
							->Present(
								0, 0
						), "Presentation failed");
						_first = false;
					} else {
						auto toPoint = point + vertex;
						RECT rect{
							ceil(point[0]), ceil(point[1]),
							ceil(toPoint[0]), ceil(toPoint[1])
						};
						DXGI_PRESENT_PARAMETERS params{
							1, &rect,
							nullptr,
							nullptr
						};
						::Windows::ThrowIfFailed(_state.get_SwapChain()
							->Present1(
								0U, 0U,
								&params
						), "Presentation failed");
					}
				};
				inline void Invalidate(vector_t const &point, vector_t const &vertex) {
					RECT rect{
						floor(point[0]), ceil(point[1]),
						floor(vertex[0]), ceil(vertex[1])
					};
					::Windows::InvalidateRect(
						_state.get_hWnd(), &rect, FALSE);
					//::Windows::UpdateWindow(
					//	_state.get_hWnd());
				};

			private:
				TState &_state;
				TQueue &_queue;
				TStopwatch &_stopwatch;
				bool _first;
			};
		}
	}
}

#endif
