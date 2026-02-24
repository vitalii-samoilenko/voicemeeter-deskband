#ifndef VOICEMEETER_UI_GRAPHICS_FRAME_HPP
#define VOICEMEETER_UI_GRAPHICS_FRAME_HPP

#include <array>
#include <utility>

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
			class Frame final {
			public:
				inline Frame(
					TSurface &surface,
					TState &state,
					TQueue &queue,
					TStopwatch &stopwatch,
					size_t layers)
					: _surface{ surface }
					, _state{ state }
					, _queue{ queue }
					, _stopwatch{ stopwatch }
					, _frameBinder{ this }
					, _token{ surface.Subscribe<Frame>() }
					, _layers_size{ layers }
					, _point{ 0, 0 }
					, _vertex{ 0, 0 }
					, _invalidFrom{ Inf, Inf }
					, _invalidTo{ 0, 0 } {
					_token.on_reallocate(_frameBinder);
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
					_point = value;
				};
				inline void set_Size(vector_t const &value) {
					for (size_t slot{ 0 }; slot < TState::SlotsSize; ++slot) {
						if (_state.get_slots_Fence(slot)
								->GetCompletedValue()
							< _state.get_slots_Count(slot)) {
							::Windows::ThrowIfFailed(_state.get_slots_Fence(slot)
								->SetEventOnCompletion(
									_state.get_slots_Count(slot),
									_state.get_slots_hEvent(slot)
							), "Event signaling failed");
							::Windows::WaitForSingleObject(
								_state.get_slots_hEvent(slot), INFINITE);
						}
					}
					D3D12_HEAP_PROPERTIES textureHeapProps{
						D3D12_HEAP_TYPE_DEFAULT,
						D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
						D3D12_MEMORY_POOL_UNKNOWN,
						0, 0
					};
					D3D12_RESOURCE_DESC textureDesc{
						D3D12_RESOURCE_DIMENSION_TEXTURE2D,
						0,
						static_cast<UINT>(max(pop(ceil(value[0])), 8)),
						static_cast<UINT>(max(pop(ceil(value[1])), 8) * _layers_size),
						1, 1,
						DXGI_FORMAT_R16G16B16A16_FLOAT,
						DXGI_SAMPLE_DESC{
							1, 0
						},
						D3D12_TEXTURE_LAYOUT_UNKNOWN,
						D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
					};
					::Windows::ThrowIfFailed(_surface.get_Device()
						->CreateCommittedResource(
							&textureHeapProps, D3D12_HEAP_FLAG_NONE, &textureDesc,
							D3D12_RESOURCE_STATE_RENDER_TARGET,
							nullptr,
							IID_PPV_ARGS(_state.geta_layers_RenderTarget())
					), "Texture creation failed");
					_surface.get_Device()
						->CreateRenderTargetView(
							_state.get_layers_RenderTarget(),
							nullptr, _state.get_layers_hRenderTarget());
					D3D12_SHADER_RESOURCE_VIEW_DESC hTextureDesc{
						DXGI_FORMAT_R16G16B16A16_FLOAT,
						D3D12_SRV_DIMENSION_TEXTURE2D,
						D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING
					};
					hTextureDesc.Texture2D.MipLevels = 1U;
					_surface.get_Device()
						->CreateShaderResourceView(
							_state.get_layers_RenderTarget(),
							&hTextureDesc,
							_state.get_blender_hTextureHeap()
								->GetCPUDescriptorHandleForHeapStart());
					_vertex = value;
				};

				inline void Invalidate(vector_t const &point, vector_t const &vertex) {
					auto lessFrom = point < _invalidFrom;
					auto to = point + vertex;
					auto greaterTo = _invalidTo < to;
					_invalidFrom[lessFrom] = point[lessFrom];
					_invalidTo[greaterTo] = to[greaterTo];
				};
				inline void Clear(vector_t const &point, vector_t const &vertex) {
					size_t slot{ _state.get_slots_Current() };
					D3D12_RECT rect{
						static_cast<LONG>(pop(floor(point[0]))),
						static_cast<LONG>(pop(floor(point[1]))),
						static_cast<LONG>(pop(ceil(point[0] + vertex[0]))),
						static_cast<LONG>(pop(ceil(point[1] + vertex[1])))
					};
					FLOAT transparent[]{ 0.F, 0.F, 0.F, 0.F };
					_state.get_slots_CommandList(slot)
						->ClearRenderTargetView(
							_state.get_layers_hRenderTarget(),
							transparent,
							1U, &rect);
				};

				inline void Update() {
					_stopwatch.Lap();
					if (_queue.empty()) {
						return;
					}
					TQueue queue{ ::std::move(_queue) };
					size_t slot{ _state.inc_slots_Current() };
					if (_state.get_slots_Fence(slot)
							->GetCompletedValue()
						< _state.get_slots_Count(slot)) {
						::Windows::ThrowIfFailed(_state.get_slots_Fence(slot)
							->SetEventOnCompletion(
								_state.get_slots_Count(slot),
								_state.get_slots_hEvent(slot)
						), "Event signaling failed");
						::Windows::WaitForSingleObject(
							_state.get_slots_hEvent(slot), INFINITE);
					}
					::Windows::ThrowIfFailed(_state.get_slots_CommandAllocator(slot)
						->Reset(
					), "Command allocator reset failed");
					::Windows::ThrowIfFailed(_state.get_slots_CommandList(slot)
						->Reset(
							_state.get_slots_CommandAllocator(slot),
							_state.get_layers_State()
					), "Command list reset failed");
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
					D3D12_CPU_DESCRIPTOR_HANDLE hRenderTarget{ _state.get_layers_hRenderTarget() };
					_state.get_slots_CommandList(slot)
						->OMSetRenderTargets(1, &hRenderTarget, FALSE, nullptr);
					for (auto &bundle : queue) {
						bundle->operator()();
					}
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
					RECT rect{
						static_cast<LONG>(pop(floor(_point[0] + _invalidFrom[0]))),
						static_cast<LONG>(pop(floor(_point[1] + _invalidFrom[1]))),
						static_cast<LONG>(pop(ceil(_point[0] + _invalidTo[0]))),
						static_cast<LONG>(pop(ceil(_point[1] + _invalidTo[1])))
					};
					::Windows::InvalidateRect(
						_surface.get_hWnd(), &rect, FALSE);
					_invalidFrom[0] = Inf;
					_invalidFrom[1] = Inf;
					_invalidTo[0] = 0;
					_invalidTo[1] = 0;
				};
				inline void Present(vector_t const &point, vector_t const &vertex) {
					vector_t from{
						max(point[0], _point[0]),
						max(point[1], _point[1])
					};
					vector_t to{
						min(point[0] + vertex[0], _point[0] + _vertex[0]),
						min(point[1] + vertex[1], _point[1] + _vertex[1])
					};
					if (is_any(to < from)) {
						return;
					}
					size_t slot{ _state.inc_slots_Current() };
					size_t buffer{
						_surface.get_SwapChain()
							->GetCurrentBackBufferIndex()
					};
					if (_state.get_slots_Fence(slot)
							->GetCompletedValue()
						< _state.get_slots_Count(slot)) {
						::Windows::ThrowIfFailed(_state.get_slots_Fence(slot)
							->SetEventOnCompletion(
								_state.get_slots_Count(slot),
								_state.get_slots_hEvent(slot)
						), "Event signaling failed");
						::Windows::WaitForSingleObject(
							_state.get_slots_hEvent(slot), INFINITE);
					}
					::Windows::ThrowIfFailed(_state.get_slots_CommandAllocator(slot)
						->Reset(
					), "Command allocator reset failed");
					::Windows::ThrowIfFailed(_state.get_slots_CommandList(slot)
						->Reset(
							_state.get_slots_CommandAllocator(slot),
							_state.get_blender_State()
					), "Command list reset failed");
					::std::array<D3D12_RESOURCE_BARRIER, 2> barriers{
						D3D12_RESOURCE_BARRIER{
							D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
							D3D12_RESOURCE_BARRIER_FLAG_NONE,
							D3D12_RESOURCE_TRANSITION_BARRIER{
								_state.get_layers_RenderTarget(),
								D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
								D3D12_RESOURCE_STATE_RENDER_TARGET,
								D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
							}
						},
						D3D12_RESOURCE_BARRIER{
							D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
							D3D12_RESOURCE_BARRIER_FLAG_NONE,
							D3D12_RESOURCE_TRANSITION_BARRIER{
								_surface.get_RenderTarget(buffer),
								D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
								D3D12_RESOURCE_STATE_PRESENT,
								D3D12_RESOURCE_STATE_RENDER_TARGET,
							}
						},
					};
					_state.get_slots_CommandList(slot)
						->ResourceBarrier(barriers.size(), &barriers[0]);
					_state.get_slots_CommandList(slot)
						->SetGraphicsRootSignature(
							_state.get_blender_RootSignature());
					ID3D12DescriptorHeap *hTextureHeap{ _state.get_blender_hTextureHeap() };
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
					D3D12_CPU_DESCRIPTOR_HANDLE hRenderTarget{
						_surface.get_hRenderTarget(buffer)
					};
					_state.get_slots_CommandList(slot)
						->OMSetRenderTargets(1, &hRenderTarget, FALSE, nullptr);
					D3D12_RECT scissor{
						static_cast<LONG>(pop(floor(from[0]))),
						static_cast<LONG>(pop(floor(from[1]))),
						static_cast<LONG>(pop(ceil(to[0]))),
						static_cast<LONG>(pop(ceil(to[1])))
					};
					_state.get_slots_CommandList(slot)
						->RSSetScissorRects(1, &scissor);
					D3D12_VIEWPORT viewport{
						static_cast<FLOAT>(from[0]) / One,
						static_cast<FLOAT>(from[1]) / One,
						static_cast<FLOAT>(to[0] - from[0]) / One,
						static_cast<FLOAT>(to[1] - from[1]) / One
					};
					_state.get_slots_CommandList(slot)
						->RSSetViewports(1, &viewport);
					FLOAT u{ static_cast<FLOAT>(from[0] - _point[0]) / _vertex[0] };
					FLOAT v{ static_cast<FLOAT>(from[1] - _point[1]) / (_vertex[1] * _layers_size) };
					::std::array<FLOAT, 5> constants{
						u,
						v + static_cast<FLOAT>(to[1] - from[1]) / (_vertex[1] * _layers_size),
						u + static_cast<FLOAT>(to[0] - from[0]) / _vertex[0],
						v,
						1.F / _layers_size
					};
					UINT layers{ static_cast<UINT>(_layers_size) };
					_state.get_slots_CommandList(slot)
						->SetGraphicsRoot32BitConstants(
							0,
							4U, &constants[0],
							0);
					_state.get_slots_CommandList(slot)
						->SetGraphicsRoot32BitConstants(
							1,
							1U, &layers,
							0);
					_state.get_slots_CommandList(slot)
						->SetGraphicsRoot32BitConstants(
							2,
							1U, &constants[4],
							0);
					_state.get_slots_CommandList(slot)
						->DrawInstanced(4, 1, 0, 0);
					barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
					barriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
					barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
					barriers[1].Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
					_state.get_slots_CommandList(slot)
						->ResourceBarrier(barriers.size(), &barriers[0]);
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
				};

			private:
				class FrameBinder final {
				public:
					inline explicit FrameBinder(Frame *that)
						: that{ that } {

					};
					FrameBinder() = delete;
					FrameBinder(FrameBinder const &) = delete;
					FrameBinder(FrameBinder &&) = delete;

					inline ~FrameBinder() = default;

					FrameBinder & operator=(FrameBinder const &) = delete;
					FrameBinder & operator=(FrameBinder &&) = delete;

					inline void Bind(TSurface &target) {

					};
					inline void Unbind(TSurface &target) {
						for (size_t slot{ 0 }; slot < TState::SlotsSize; ++slot) {
							if (that->_state.get_slots_Fence(slot)
									->GetCompletedValue()
								< that->_state.get_slots_Count(slot)) {
								::Windows::ThrowIfFailed(that->_state.get_slots_Fence(slot)
									->SetEventOnCompletion(
										that->_state.get_slots_Count(slot),
										that->_state.get_slots_hEvent(slot)
								), "Event signaling failed");
								::Windows::WaitForSingleObject(
									that->_state.get_slots_hEvent(slot), INFINITE);
							}
						}
					};

				private:
					Frame *that;
				};

				friend class FrameBinder;

				TSurface &_surface;
				TState &_state;
				TQueue &_queue;
				TStopwatch &_stopwatch;
				FrameBinder _frameBinder;
				typename TSurface::token _token;
				size_t _layers_size;
				vector_t _point;
				vector_t _vertex;
				vector_t _invalidFrom;
				vector_t _invalidTo;
			};
		}
	}
}

#endif
