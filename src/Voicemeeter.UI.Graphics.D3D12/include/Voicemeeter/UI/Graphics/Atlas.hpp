#ifndef VOICEMEETER_UI_GRAPHICS_ATLAS_HPP
#define VOICEMEETER_UI_GRAPHICS_ATLAS_HPP

#include <array>

#include "wheel.hpp"

#include "Windows/API.hpp"
#include <d3d12.h>

#include "Voicemeeter/UI/Layouts/Atlas.hpp"

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
					vector_t const &srcPoint, vector_t const &srcVertex,
					vector_t const &dstPoint, vector_t const &dstVertex,
					vector_t const &color) const {
					size_t slot{ _state.get_slots_Current() };
					D3D12_RECT scissor{
						static_cast<LONG>(pop(floor(dstPoint[0]))),
						static_cast<LONG>(pop(floor(dstPoint[1]))),
						static_cast<LONG>(pop(ceil(dstPoint[0] + dstVertex[0]))),
						static_cast<LONG>(pop(ceil(dstPoint[1] + dstVertex[1])))
					};
					_state.get_slots_CommandList(slot)
						->RSSetScissorRects(1, &scissor);
					D3D12_VIEWPORT viewport{
						static_cast<FLOAT>(scissor.left),
						static_cast<FLOAT>(scissor.top),
						static_cast<FLOAT>(scissor.right - scissor.left),
						static_cast<FLOAT>(scissor.bottom - scissor.top)
					};
					_state.get_slots_CommandList(slot)
						->RSSetViewports(1, &viewport);
					FLOAT r0{ static_cast<FLOAT>(srcVertex[0]) / dstVertex[0] };
					FLOAT r1{ static_cast<FLOAT>(srcVertex[1]) / dstVertex[1] };
					FLOAT u{ static_cast<FLOAT>(srcPoint[0]) / Layouts::Atlas::Width };
					FLOAT v{ static_cast<FLOAT>(srcPoint[1]) / Layouts::Atlas::Height };
					::std::array<FLOAT, 9> constants{
						u - frac(dstPoint[0]) * r0 / Layouts::Atlas::Width,
						v - frac(dstPoint[1]) * r1 / Layouts::Atlas::Height,
						u + static_cast<FLOAT>(srcVertex[0]) / Layouts::Atlas::Width
						+ frac(push(2) - frac(dstPoint[0]) - frac(dstVertex[0]))
						* r0 / Layouts::Atlas::Width,
						v + static_cast<FLOAT>(srcVertex[1]) / Layouts::Atlas::Height
						+ frac(push(2) - frac(dstPoint[1]) - frac(dstVertex[1]))
						* r1 / Layouts::Atlas::Height,
						Layouts::Atlas::Range::Width / ::std::max(r0, r1),
						static_cast<FLOAT>(color[0]) / push(255),
						static_cast<FLOAT>(color[1]) / push(255),
						static_cast<FLOAT>(color[2]) / push(255),
						static_cast<FLOAT>(color[3]) / push(255)
					};
					_state.get_slots_CommandList(slot)
						->SetGraphicsRoot32BitConstants(
							0,
							4U, &constants[0],
							0);
					_state.get_slots_CommandList(slot)
						->SetGraphicsRoot32BitConstants(
							1,
							1U, &constants[4],
							0);
					_state.get_slots_CommandList(slot)
						->SetGraphicsRoot32BitConstants(
							2,
							4U, &constants[5],
							0);
					_state.get_slots_CommandList(slot)
						->DrawInstanced(4, 1, 0, 0);
				};

			private:
				TState &_state;
			};
		}
	}
}

#endif
