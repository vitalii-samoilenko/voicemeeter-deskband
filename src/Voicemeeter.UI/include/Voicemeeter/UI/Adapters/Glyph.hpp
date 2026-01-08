#ifndef VOICEMEETER_UI_ADAPTERS_GLYPH_HPP
#define VOICEMEETER_UI_ADAPTERS_GLYPH_HPP

#include <utility>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			template<typename TBundle, typename TScale>
			class Glyph : public TBundle {
			public:
				template<typename... Args>
				inline Glyph(
					::std::valarray<int> &&baseVertex,
					TScale &&scale = TScale{},
					Args &&...args)
					: TBundle{ ::std::forward<Args>(args)... }
					, _baseVertex{ ::std::move(baseVertex) }
					, _scale{ ::std::move(scale) } {

				};
				Glyph() = delete;
				Glyph(Glyph const &) = delete;
				Glyph(Glyph &&) = delete;

				inline ~Glyph() = default;

				Glyph & operator=(Glyph const &) = delete;
				Glyph & operator=(Glyph &&) = delete;

				inline ::std::valarray<int> const & get_Position() const {
					return TBundle::get_FramePosition();
				};
				inline ::std::valarray<int> const & get_Size() const {
					return TBundle::get_FrameSize();
				};
				inline ::std::valarray<int> const & get_BaseSize() const {
					return _baseVertex;
				};

				inline void Redraw(::std::valarray<int> const &point, ::std::valarray<int> const &vertex) {
					TBundle::set_Invalid();
				};
				inline void Rescale(::std::valarray<int> const &vertex) {
					TBundle::set_FrameSize(_scale(vertex, _baseVertex));
				};
				inline void Move(::std::valarray<int> const &point) {
					TBundle::set_FramePosition(point);
				};

			private:
				::std::valarray<int> _baseVertex;
				TScale _scale;
			};
		}
	}
}

#endif
