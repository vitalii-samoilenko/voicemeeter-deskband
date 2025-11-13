#ifndef VOICEMEETER_UI_GRAPHICS_BUNDLES_KNOB_HPP
#define VOICEMEETER_UI_GRAPHICS_BUNDLES_KNOB_HPP

#include <bitset>
#include <string_view>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			namespace Bundles {
				template<typename TPalette>
				class Knob {
				public:
					inline explicit Knob(TPalette &palette)
						: _palette{ palette }
						, _changes{}
						, _framePoint{ 0, 0 }
						, _frameVertex{ 0, 0 }
						, _frameAtlasPoint{ 0, 0 }
						, _frameAtlasVertex{ 0, 0 }
						, _frameRgba{ 0, 0, 0, 0 }
						, _indicatorDegree{}
						, _indicatorPoint{ 0, 0 }
						, _indicatorVertex{ 0, 0 }
						, _indicatorAtlasPoint{ 0, 0 }
						, _indicatorAtlasVertex{ 0, 0 }
						, _indicatorRgba{ 0, 0, 0, 0 }
						, _indicatorTextPoint{ 0, 0 }
						, _indicatorTextVertex{ 0, 0 }
						, _indicatorTextColor{ 0, 0, 0, 0 }

					};
					Knob() = delete;
					Knob(Knob const &) = delete;
					Knob(Knob &&) = delete;

					inline ~Knob() = default;

					Knob & operator=(Knob const &) = delete;
					Knob & operator=(Knob &&) = delete;

					inline void set_Position(::std::valarray<int> const &point) {
						OnSet(_framePoint, point, framePoint);
					};
					inline void set_Size(::std::valarray<int> const &vertex) {
						OnSet(_frameVertex, vertex, frameVertex);
					};
					inline void set_Color(::std::valarray<int> const &rgba) {
						OnSet(_frameRgba, rgba, frameRgba);
					};
					inline void set_Angle(int degree) {
						OnSet(_indicatorDegree, degree, indicatorDegree);
					};
					inline void set_IndicatorColor(::std::valarray<int> const &rgba) {
						OnSet(_indicatorRgba, rgba, indicatorRgba);
					};
					inline void set_Label(::std::string_view text) {

					};
					inline void set_LabelColor(::std::valarray<int> const &rgba) {

					};

					inline void Invalidate() {
						OnInvalidate(renderTarget);
					};
					inline void Execute() {
						if (_changes.test(frameVertex)) {
							_changes.set(framePoint);
						}
						if (_changes.test(framePoint)) {
							_changes.set(indicatorDegree);
						}
						if (_changes.test(indicatorDegree) {
						}
						_changes.reset();
						_palette.get_Atlas()
							.FillSDF(
								_frameAtlasPoint, _frameAtlasVertex,
								_framePoint, _frameVertex,
								_frameRgba);
						_palette.get_Atlas()
							.FillSDF(
								_indicatorAtlasPoint, _indicatorAtlasVertex,
								_indicatorPoint, _indicatorVertex,
								_indicatorRgba);
					};

				protected:
					inline TPalette & get_Palette() {
						return _palette;
					};
					inline ::std::valarray<int> const & get_Position() const {
						return _framePoint;
					};
					inline ::std::valarray<int> const & get_Size() const {
						return _frameVertex;
					};

				private:
					enum flags : size_t {
						framePoint,
						frameVertex,
						frameRgba,
						indicatorDegree,
						indicatorRgba,
						indicatorText,
						indicatorTextRgba,
						renderTarget
					};

					TPalette &_palette;
					::std::bitset<renderTarget + 1> _changes;
					::std::valarray<int> _framePoint;
					::std::valarray<int> _frameVertex;
					::std::valarray<int> _frameAtlasPoint;
					::std::valarray<int> _frameAtlasVertex;
					::std::valarray<int> _frameRgba;
					int _indicatorDegree;
					::std::valarray<int> _indicatorPoint;
					::std::valarray<int> _indicatorVertex;
					::std::valarray<int> _indicatorAtlasPoint;
					::std::valarray<int> _indicatorAtlasVertex;
					::std::valarray<int> _indicatorRgba;
					::std::valarray<int> _indicatorTextPoint;
					::std::valarray<int> _indicatorTextVertex;
					::std::valarray<int> _indicatorTextRgba;

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
