#ifndef VOICEMEETER_UI_GRAPHICS_BUNDLES_VBAN_HPP
#define VOICEMEETER_UI_GRAPHICS_BUNDLES_VBAN_HPP

#include <bitset>
#include <string_view>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			namespace Bundles {
				template<typename TPalette>
				class Vban {
				public:
					inline explicit Vban(TPalette &palette)
						: _palette{ palette }
						, _changes{}
						, _framePoint{ 0, 0 }
						, _frameVertex{ 0, 0 }
						, _frameAtlasPoint{ 0, 0 }
						, _frameAtlasVertex{ 0, 0 }
						, _frameRgba{ 0, 0, 0, 0 }
						, _textPoint{ 0, 0 }
						, _textVertex{ 0, 0 }
						, _textRgba{ 0, 0, 0, 0 } {

					};
					Vban() = delete;
					Vban(Vban const &) = delete;
					Vban(Vban &&) = delete;

					inline ~Vban() = default;

					Vban & operator=(Vban const &) = delete;
					Vban & operator=(Vban &&) = delete;

					inline ::std::valarray<int> const & get_Position() const {
						return _framePoint;
					};
					inline ::std::valarray<int> const & get_Size() const {
						return _frameVertex;
					};

					inline void set_Position(::std::valarray<int> const &point) {
						OnSet(_framePoint, point, framePoint);
					};
					inline void set_Size(::std::valarray<int> const &vertex) {
						OnSet(_frameVertex, vertex, frameVertex);
					};
					inline void set_Color(::std::valarray<int> const &rgba) {
						OnSet(_frameRgba, rgba, frameRgba);
					};
					inline void set_Label(::std::string_view text) {

					};
					inline void set_LabelColor(::std::valarray<int> const &rgba) {
						OnSet(_textRgba, rgba, textRgba);
					};
					inline void set_Invalid() {
						OnInvalidate(renderTarget);
					};

					inline void operator()() {
						if (_changes.test(frameVertex)) {
							_changes.set(framePoint);
						}
						if (_changes.test(framePoint)) {
						}
						_changes.reset();
						_palette.get_Atlas()
							.FillSDF(
								_frameAtlasPoint, _frameAtlasVertex,
								_framePoint, _frameVertex,
								_frameRgba);
					};

				private:
					enum flags : size_t {
						framePoint,
						frameVertex,
						frameRgba,
						text,
						textRgba,
						renderTarget
					};

					TPalette &_palette;
					::std::bitset<renderTarget + 1> _changes;
					::std::valarray<int> _framePoint;
					::std::valarray<int> _frameVertex;
					::std::valarray<int> _frameAtlasPoint;
					::std::valarray<int> _frameAtlasVertex;
					::std::valarray<int> _frameRgba;
					::std::valarray<int> _textPoint;
					::std::valarray<int> _textVertex;
					::std::valarray<int> _textRgba;

					template<typename T>
					inline void OnSet(T &dst, T const &src, flags property) {
						if (src == dst) {
							return;
						}
						dst = src;
						OnInvalidate(property);
					};
					inline void OnInvalidate(flags property) {
						if (_changes.none()) {
							_palette.get_Queue()
								+= *this;
						}
						_changes.set(property);
					};
				};
			}
		}
	}
}

#endif
