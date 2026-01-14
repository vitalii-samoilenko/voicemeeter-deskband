#ifndef VOICEMEETER_UI_GRAPHICS_ATLAS_HPP
#define VOICEMEETER_UI_GRAPHICS_ATLAS_HPP

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
						to_FLOAT(dstPoint[0]), to_FLOAT(dstPoint[1]),
						to_FLOAT(dstVertex[0]), to_FLOAT(dstVertex[1])
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
						to_FLOAT(color[0]),
						to_FLOAT(color[1]),
						to_FLOAT(color[2]),
						to_FLOAT(color[3]),
						to_FLOAT(srcPoint[0] / m_pAtlas->get_Width()),
						to_FLOAT(srcPoint[1] / m_pAtlas->get_Height()),
						to_FLOAT(srcVertex[0] / m_pAtlas->get_Width()),
						to_FLOAT(srcVertex[1] / m_pAtlas->get_Height())
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

				static FLOAT to_FLOAT(num_t n) {
					return static_cast<FLOAT>(n) / SCALING_FACTOR;
				};
				static num_t floor(num_t n) {
					return n / SCALING_FACTOR;
				};
				static num_t ceil(num_t n) {
					return (n + SCALING_FACTOR - 1) / SCALING_FACTOR;
				};
			};
		}
	}
}

#endif
