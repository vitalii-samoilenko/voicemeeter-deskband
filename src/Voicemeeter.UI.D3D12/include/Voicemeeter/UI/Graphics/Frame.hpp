#ifndef VOICEMEETER_UI_GRAPHICS_FRAME_HPP
#define VOICEMEETER_UI_GRAPHICS_FRAME_HPP

#include <array>
#include <cmath>
#include <utility>

#include "Windows/API.hpp"

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
					, _first{ true }
					, _fromPoint{ 0, 0 }
					, _toPoint{ 0, 0 } {

				};
				Frame() = delete;
				Frame(Frame const &) = delete;
				Frame(Frame &&) = delete;

				inline ~Frame() = default;

				Frame & operator=(Frame const &) = delete;
				Frame & operator=(Frame &&) = delete;

				inline vector_t const & get_Size() const {
					return vector_t{ 8, 8 };
				};
				inline void set_Size(vector_t const &vertex) {
					for (size_t frame{ 0 }; frame < _state.get_Frames(); ++frame) {
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
							static_cast<UINT>(::std::max(8,
								(vertex[0] + SCALING_FACTOR - 1) / SCALING_FACTOR)),
							static_cast<UINT>(::std::max(8,
								(vertex[1] + SCALING_FACTOR - 1) / SCALING_FACTOR)),
							DXGI_FORMAT_UNKNOWN,
							0
					), "Swap chain resize failed");
					for (size_t frame{ 0 }; frame < _state.get_Frames(); ++frame) {
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
					if (_queue.empty()) {
						return;
					}
					TQueue queue{ ::std::move(_queue) };
					_stopwatch.Lap();
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
	::std::array<ID3D12DescriptorHeap *, 1> hTextureHeaps{
		_state.get_hTextureHeap()
	};
	_state.get_CommandList(frame)
		->SetDescriptorHeaps(
			static_cast<UINT>(hTextureHeaps.size()),
			hTextureHeaps.data());
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
	for (Graphics::Bundle* pBundle : queue) {
		const ::std::valarray<double>& dirtyPoint{ pBundle->get_Position() };
		const ::std::valarray<double>& dirtyVertex{ pBundle->get_Size() };
		cRect.push_back({
			static_cast<LONG>(::std::floor(dirtyPoint[0])),
			static_cast<LONG>(::std::floor(dirtyPoint[1])),
			static_cast<LONG>(::std::ceil(::std::min(canvasVertex[0], dirtyPoint[0] + dirtyVertex[0]))),
			static_cast<LONG>(::std::ceil(::std::min(canvasVertex[1], dirtyPoint[1] + dirtyVertex[1])))
		});
		pBundle->Execute();
	}
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	m_pPalette->get_Instrumentation()
		.get_pCommandList(frame)
			->ResourceBarrier(1U, &barrier);
	::Windows::ThrowIfFailed(m_pPalette->get_Instrumentation()
		.get_pCommandList(frame)
			->Close(
	), "Command list close failed");
	::std::array<ID3D12CommandList*, 1> cpCommandList{
		m_pPalette->get_Instrumentation()
			.get_pCommandList(frame)
	};
	m_pPalette->get_Instrumentation()
		.get_pCommandQueue()
			->ExecuteCommandLists(
				static_cast<UINT>(cpCommandList.size()),
				cpCommandList.data());
	if (m_first) {
		::Windows::ThrowIfFailed(m_pPalette->get_Instrumentation()
			.get_pSwapChain()
				->Present(
					0U, 0U
		), "Presentation failed");
		m_first = false;
	} else {
		DXGI_PRESENT_PARAMETERS params{
			static_cast<UINT>(cRect.size()), cRect.data(),
			nullptr,
			nullptr
		};
		::Windows::ThrowIfFailed(m_pPalette->get_Instrumentation()
			.get_pSwapChain()
				->Present1(
					0U, 0U,
					&params
		), "Presentation failed");
	}
	m_pPalette->get_Instrumentation()
		.get_pCommandQueue()
			->Signal(
				m_pPalette->get_Instrumentation()
					.get_pFence(frame),
				++m_pPalette->get_Instrumentation()
					.get_Count(frame));
				};
				inline void Present(vector_t const &point, vector_t const &vertex) {
				};
				inline void Invalidate(vector_t const &point, vector_t const &vertex) {
					auto invalidFrom = point < _fromPoint;
					_fromPoint[invalidFrom] = point[invalidFrom];
					auto toPoint = point + vertex;
					auto invalidTo = _toPoint < toPoint;
					_toPoint[invalidTo] = toPoint[invalidTo];
				};

			private:
				TState &_state;
				TQueue &_queue;
				TStopwatch &_stopwatch;
				bool _first;
				vector_t _fromPoint;
				vector_t _toPoint;
			};
		}
	}
}

#endif
