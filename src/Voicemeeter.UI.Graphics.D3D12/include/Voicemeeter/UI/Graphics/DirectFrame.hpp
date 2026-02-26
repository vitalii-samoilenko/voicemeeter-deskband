#ifndef VOICEMEETER_UI_GRAPHICS_DIRECTFRAME_HPP
#define VOICEMEETER_UI_GRAPHICS_DIRECTFRAME_HPP

#include <array>
#include <utility>
#include <vector>

#include "wheel.hpp"

#include "Windows/API.hpp"
#include <d3d12.h>

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			template<
				typename TSurface,
				typename TState,
				typename TQueue,
				typename TStopwatch>
			class CachedFrame final {
			public:
				inline Frame(
					TSurface &surface,
					TState &state,
					TQueue &queue,
					TStopwatch &stopwatch)
					: _surface{ surface }
					, _state{ state }
					, _queue{ queue }
					, _stopwatch{ stopwatch }
					, _point{ 0, 0 }
					, _vertex{ 0, 0 } {

				};
				Frame() = delete;
				Frame(Frame const &) = delete;
				Frame(Frame &&) = delete;

				inline ~Frame() = default;

				Frame & operator=(Frame const &) = delete;
				Frame & operator=(Frame &&) = delete;

				inline vector_t const & get_Position() const {
					return _point;
				};
				inline vector_t const & get_Size() const {
					return _vertex;
				};

				inline void set_Position(vector_t const &value) {

				};
				inline void set_Size(vector_t const &value) {
					_surface.Resize(value);
					_vertex = value;
				};

				inline void Update() {
					_stopwatch.Lap();
					if (_queue.empty()) {
						return;
					}
					TQueue queue{ ::std::move(_queue) };
					size_t slot{ _state.inc_slots_Current() };
					size_t buffer{ _surface.get_buffers_Current() };
					if (_state.get_slots_Fence(slot)
							->GetCompletedValue()
						< _state.get_slots_Count(slot)) {
						::Windows::ThrowIfFailed(_state.get_slots_Fence(slot)
							->SetEventOnCompletion(
								_state.get_slots_Count(slot),
								_state.get_hEvent()
						), "Event signaling failed");
						::Windows::WaitForSingleObject(
							_state.get_hEvent(), INFINITE);
					}
					::Windows::ThrowIfFailed(_state.get_slots_CommandAllocator(slot)
						->Reset(
					), "Command allocator reset failed");
					::Windows::ThrowIfFailed(_state.get_slots_CommandList(slot)
						->Reset(
							_state.get_slots_CommandAllocator(slot),
							_state.get_layers_State()
					), "Command list reset failed");
					D3D12_RESOURCE_BARRIER barrier{
						D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
						D3D12_RESOURCE_BARRIER_FLAG_NONE,
						D3D12_RESOURCE_TRANSITION_BARRIER{
							_surface.get_buffers_RenderTarget(buffer),
							D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
							D3D12_RESOURCE_STATE_PRESENT,
							D3D12_RESOURCE_STATE_RENDER_TARGET
						}
					};
					_state.get_slots_CommandList(slot)
						->ResourceBarrier(1U, &barrier);
					_state.get_slots_CommandList(slot)
						->SetGraphicsRootSignature(
							_state.get_layers_RootSignature());
					ID3D12DescriptorHeap *hTextureHeap{ _state.get_layers_hTextureHeap() };
					_state.get_slots_CommandList(slot)
						->SetDescriptorHeaps(1, &hTextureHeap);
					_state.get_slots_CommandList(slot)
						->SetGraphicsRootDescriptorTable(
							3, hTextureHeap->GetGPUDescriptorHandleForHeapStart());
					_state.get_slots_CommandList(slot)
						->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
					D3D12_VERTEX_BUFFER_VIEW hSquareBuffer{ _state.get_hSquareBuffer() };
					_state.get_slots_CommandList(slot)
						->IASetVertexBuffers(0, 1, &hSquareBuffer);
					D3D12_CPU_DESCRIPTOR_HANDLE hRenderTarget{ _surfce.get_buffers_hRenderTarget(buffer) };
					_state.get_slots_CommandList(slot)
						->OMSetRenderTargets(1, &hRenderTarget, FALSE, nullptr);
					for (auto &bundle : queue) {
						bundle->operator()();
					}
					barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
					barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
					_state.get_slots_CommandList(slot)
						->ResourceBarrier(1U, &barrier);
					::Windows::ThrowIfFailed(_state.get_slots_CommandList(slot)
						->Close(
					), "Command list close failed");
					ID3D12CommandList *commandList{ _state.get_slots_CommandList(slot) };
					_surface.get_CommandQueue()
						->ExecuteCommandLists(1, &commandList);
					_surface.get_CommandQueue()
						->Signal(
							_state.get_slots_Fence(slot),
							_state.inc_slots_Count(slot));
					_surface.Present();
				};
				inline void Present(vector_t const &point, vector_t const &vertex) {

				};

				inline void Invalidate(vector_t const &point, vector_t const &vertex) {
					_surface.Invalidate(point, vertex);
				};
				inline void Clear(vector_t const &point, vector_t const &vertex) {
					size_t slot{ _state.get_slots_Current() };
					size_t buffer{ _surface.get_buffers_Current() };
					D3D12_RECT rect{
						static_cast<LONG>(pop(floor(point[0]))),
						static_cast<LONG>(pop(floor(point[1]))),
						static_cast<LONG>(pop(ceil(point[0] + vertex[0]))),
						static_cast<LONG>(pop(ceil(point[1] + vertex[1])))
					};
					FLOAT transparent[]{ 0.F, 0.F, 0.F, 0.F };
					_state.get_slots_CommandList(slot)
						->ClearRenderTargetView(
							_surface.get_buffers_hRenderTarget(buffer),
							transparent,
							1U, &rect);
				};

			private:
				TSurface &_surface;
				TState &_state;
				TQueue &_queue;
				TStopwatch &_stopwatch;
				vector_t _point;
				vector_t _vertex;
			};
		}
	}
}

#endif
