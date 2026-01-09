#ifndef VOICEMEETER_UI_GRAPHICS_BUNDLES_KNOB_HPP
#define VOICEMEETER_UI_GRAPHICS_BUNDLES_KNOB_HPP

#include <bitset>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			namespace Bundles {
				template<typename TToolkit>
				class Knob {
				public:
					inline explicit Knob(TToolkit &toolkit)
						: _toolkit{ toolkit }
						, _slot{
							_toolkit.get_Queue()
								.reserve()
						}
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
						, _indicatorRgba{ 0, 0, 0, 0 } {

					};
					Knob() = delete;
					Knob(Knob const &) = delete;
					Knob(Knob &&) = delete;

					inline ~Knob() = default;

					Knob & operator=(Knob const &) = delete;
					Knob & operator=(Knob &&) = delete;

					inline ::std::valarray<int> const & get_FramePosition() const {
						return _framePoint;
					};
					inline ::std::valarray<int> const & get_FrameSize() const {
						return _frameVertex;
					};

					inline void set_FramePosition(::std::valarray<int> const &point) {
						OnSet(_framePoint, point, FramePoint);
					};
					inline void set_FrameSize(::std::valarray<int> const &vertex) {
						OnSet(_frameVertex, vertex, FrameVertex);
					};
					inline void set_FrameColor(::std::valarray<int> const &rgba) {
						OnSet(_frameRgba, rgba, FrameRgba);
					};
					inline void set_IndicatorAngle(int degree) {
						OnSet(_indicatorDegree, degree, IndicatorDegree);
					};
					inline void set_IndicatorColor(::std::valarray<int> const &rgba) {
						OnSet(_indicatorRgba, rgba, IndicatorRgba);
					};
					inline void set_Invalid() {
						OnInvalidate(RenderTarget);
					};

					inline void operator()() {
						if (_changes.test(FrameVertex)) {
							_changes.set(FramePoint);
						}
						if (_changes.test(FramePoint)) {
							_changes.set(IndicatorDegree);
						}
						if (_changes.test(IndicatorDegree) {
						}
						_changes.reset();
						_slot.reset();
						_toolkit.get_Atlas()
							.FillSDF(
								_frameAtlasPoint, _frameAtlasVertex,
								_framePoint, _frameVertex,
								_frameRgba);
						_toolkit.get_Atlas()
							.FillSDF(
								_indicatorAtlasPoint, _indicatorAtlasVertex,
								_indicatorPoint, _indicatorVertex,
								_indicatorRgba);
					};

				protected:
					inline slot_t & get_Slot() {
						return _slot;
					};

				private:
					using slot_t = decltype(_toolkit.get_Queue())::slot;
					enum flags_t : size_t {
						FramePoint,
						FrameVertex,
						FrameRgba,
						IndicatorDegree,
						IndicatorRgba,
						RenderTarget
					};

					TToolkit &_toolkit;
					slot_t _slot;
					::std::bitset<RenderTarget + 1> _changes;
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

					template<typename T>
					inline void OnSet(T &dst, T const &src, flags_t property) {
						if (src == dst) {
							return;
						}
						dst = src;
						OnInvalidate(property);
					};
					inline void OnInvalidate(flags_t property) {
						if (_changes.none()) {
							_slot.overwrite(*this);
						}
						_changes.set(property);
					};
				};
			}
		}
	}
}

#endif
