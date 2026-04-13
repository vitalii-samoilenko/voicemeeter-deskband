#ifndef WUI_GRAPHICS_ATLAS_HPP
#define WUI_GRAPHICS_ATLAS_HPP

#include <algorithm>
#include <array>

#include "math.hpp"

#include "Windows/API.hpp"
#include "Windows/D3D12.hpp"

#include "WUI/Layouts/Atlas.hpp"

namespace WUI {
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
				vec_t const &srcPoint, vec_t const &srcVertex,
				vec_t const &dstPoint, vec_t const &dstVertex,
				vec_t const &color) const {
				size_t slot{ _state.get_slots_Current() };
				D3D12_RECT scissor{
					static_cast<LONG>(pop(floor(sub(dstPoint, 0)))),
					static_cast<LONG>(pop(floor(sub(dstPoint, 1)))),
					static_cast<LONG>(pop(ceil(sub(dstPoint, 0) + sub(dstVertex, 0)))),
					static_cast<LONG>(pop(ceil(sub(dstPoint, 1) + sub(dstVertex, 1))))
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
				FLOAT r0{ static_cast<FLOAT>(sub(srcVertex, 0)) / sub(dstVertex, 0) };
				FLOAT r1{ static_cast<FLOAT>(sub(srcVertex, 1)) / sub(dstVertex, 1) };
				FLOAT from0{ sub(srcPoint, 0) - frac(sub(dstPoint, 0)) * r0 };
				FLOAT from1{ sub(srcPoint, 1) - frac(push(2) - frac(sub(dstPoint, 1)) - frac(sub(dstVertex, 1))) * r1 };
				FLOAT to0{ sub(srcPoint, 0) + sub(srcVertex, 0) + frac(push(2) - frac(sub(dstPoint, 0)) - frac(sub(dstVertex, 0))) * r0 };
				FLOAT to1{ sub(srcPoint, 1) + sub(srcVertex, 1) + frac(sub(dstPoint, 1)) * r1 };
				::std::array<FLOAT, 9> constants{
					from0 / Layouts::Atlas::Width,
					from1 / Layouts::Atlas::Height,
					to0 / Layouts::Atlas::Width,
					to1 / Layouts::Atlas::Height,
					Layouts::Atlas::Range::Width * ::std::min(
						viewport.Width * One / (to0 - from0),
						viewport.Height * One / (to1 - from1)),
					static_cast<FLOAT>(sub(color, 0)) / push(255),
					static_cast<FLOAT>(sub(color, 1)) / push(255),
					static_cast<FLOAT>(sub(color, 2)) / push(255),
					static_cast<FLOAT>(sub(color, 3)) / push(255)
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

#endif
