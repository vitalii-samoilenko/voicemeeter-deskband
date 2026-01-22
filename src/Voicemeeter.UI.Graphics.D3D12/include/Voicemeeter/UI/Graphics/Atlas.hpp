#ifndef VOICEMEETER_UI_GRAPHICS_ATLAS_HPP
#define VOICEMEETER_UI_GRAPHICS_ATLAS_HPP

#include <array>

#include "wheel.hpp"

#include "Windows/API.hpp"
#include <d3d12.h>

#include "Voicemeeter/UI/Definitions/Atlas.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			template<typename TState>
			class Atlas final {
			public:
				inline explicit Atlas(TState &state)
					: _state{ state } {

				};
				Atlas() = delete;
				Atlas(Atlas const &) = delete;
				Atlas(Atlas &&) = delete;

				inline ~Atlas() = default;

				Atlas & operator=(Atlas const &) = delete;
				Atlas & operator=(Atlas &&) = delete;

				inline void FillSDF(
					vector_t const &dstPoint, vector_t const &dstVertex,
					vector_t const &srcPoint, vector_t const &srcVertex,
					bool blend = false) const {
					size_t frame{
						_state.get_SwapChain()
							->GetCurrentBackBufferIndex()
					};
					if (blend) {
						_state.get_CommandList(frame)
							->SetPipelineState(
								_state.get_BlendState());
					}
					D3D12_VIEWPORT viewport{
						static_cast<FLOAT>(dstPoint[0]) / One,
						static_cast<FLOAT>(dstPoint[1]) / One,
						static_cast<FLOAT>(dstVertex[0]) / One,
						static_cast<FLOAT>(dstVertex[1]) / One
					};
					_state.get_CommandList(frame)
						->RSSetViewports(1, &viewport);
					D3D12_RECT scissor{
						static_cast<LONG>(floor(point[0])),
						static_cast<LONG>(floor(point[1])),
						static_cast<LONG>(ceil(point[0] + vertex[0])),
						static_cast<LONG>(ceil(point[1] + vertex[1]))
					};
					_state.get_CommandList(frame)
						->RSSetScissorRects(1, &scissor);
					::std::array<FLOAT, 8> constants{
						static_cast<FLOAT>(color[0]) / One,
						static_cast<FLOAT>(color[1]) / One,
						static_cast<FLOAT>(color[2]) / One,
						static_cast<FLOAT>(color[3]) / One,
						static_cast<FLOAT>(srcPoint[0]) / push(ATLAS_W),
						static_cast<FLOAT>(srcPoint[1]) / push(ATLAS_H),
						static_cast<FLOAT>(srcVertex[0]) / push(ATLAS_W),
						static_cast<FLOAT>(srcVertex[1]) / push(ATLAS_H)
					};
					_state.get_CommandList(frame)
						->SetGraphicsRoot32BitConstants(
							1,
							static_cast<UINT>(constants.size()), constants.data(),
							0);
					_state.get_CommandList(frame)
						->DrawInstanced(4, 1, 0, 0);
					if (blend) {
						_state.get_CommandList(frame)
							->SetPipelineState(
								_state.get_DefaultState());
					}
				};

			private:
				TState &_state;
			};
		}
	}
}

#endif
