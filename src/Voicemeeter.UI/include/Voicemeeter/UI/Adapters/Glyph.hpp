#ifndef VOICEMEETER_UI_ADAPTERS_GLYPH_HPP
#define VOICEMEETER_UI_ADAPTERS_GLYPH_HPP

#include <utility>

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			template<
				typename TBundle,
				typename TScale>
			class Glyph : public TBundle {
			public:
				template<typename ...Args>
				inline Glyph(
					vec_t &&baseVertex,
					TScale &&scale = TScale{},
					Args &&...args)
					: TBundle{ ::std::forward<Args>(args) ... }
					, _baseVertex{ ::std::move(baseVertex) }
					, _scale{ ::std::move(scale) } {

				};
				Glyph() = delete;
				Glyph(Glyph const &) = delete;
				Glyph(Glyph &&) = delete;

				inline ~Glyph() = default;

				Glyph & operator=(Glyph const &) = delete;
				Glyph & operator=(Glyph &&) = delete;

				inline vec_t const & get_Position() const {
					return TBundle::get_FramePosition();
				};
				inline vec_t const & get_Size() const {
					return TBundle::get_FrameSize();
				};
				inline vec_t const & get_BaseSize() const {
					return _baseVertex;
				};

				inline void Redraw(vec_t const &point, vec_t const &vertex) {
					TBundle::set_Invalid();
				};
				inline void Rescale(vec_t const &vertex) {
					auto [frameVertex] = _scale(vertex, _baseVertex);
					TBundle::set_FrameSize(frameVertex);
				};
				inline void Move(vec_t const &point) {
					TBundle::set_FramePosition(point);
				};

			private:
				vec_t _baseVertex;
				TScale _scale;
			};
		}
	}
}

#endif
