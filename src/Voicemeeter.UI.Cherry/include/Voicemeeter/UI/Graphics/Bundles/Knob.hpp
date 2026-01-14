#ifndef VOICEMEETER_UI_GRAPHICS_BUNDLES_KNOB_HPP
#define VOICEMEETER_UI_GRAPHICS_BUNDLES_KNOB_HPP

#include <bitset>
#include <optional>

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			namespace Bundles {
				template<typename TToolkit>
				class Knob {
				public:
					inline explicit Knob(TToolkit &toolkit)
						: _toolkit{ toolkit }
						, _slot{}
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

					inline vector_t const & get_FramePosition() const {
						return _framePoint;
					};
					inline vector_t const & get_FrameSize() const {
						return _frameVertex;
					};
					inline vector_t const & get_FrameColor() const {
						return _frameRgba;
					};

					inline void set_FramePosition(vector_t const &point) {
						OnSet(_framePoint, point, FramePoint);
					};
					inline void set_FrameSize(vector_t const &vertex) {
						OnSet(_frameVertex, vertex, FrameVertex);
					};
					inline void set_FrameColor(vector_t const &rgba) {
						OnSet(_frameRgba, rgba, FrameRgba);
					};
					inline void set_IndicatorAngle(num_t degree) {
						OnSet(_indicatorDegree, degree, IndicatorDegree);
					};
					inline void set_IndicatorColor(vector_t const &rgba) {
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
						_toolkit.get_Frame()
							.Invalidate(_framePoint, _frameVertex);
					};

				protected:
					using slot_t = typename TToolkit::Queue::slot;

					inline slot_t & get_Slot() {
						return _slot;
					};

				private:
					enum flags_t : size_t {
						FramePoint = 0,
						FrameVertex = 1,
						FrameRgba = 2,
						IndicatorDegree = 3,
						IndicatorRgba = 4,
						RenderTarget = 5
					};

					TToolkit &_toolkit;
					::std::optional<slot_t> _slot;
					::std::bitset<RenderTarget + 1> _changes;
					vector_t _framePoint;
					vector_t _frameVertex;
					vector_t _frameAtlasPoint;
					vector_t _frameAtlasVertex;
					vector_t _frameRgba;
					num_t _indicatorDegree;
					vector_t _indicatorPoint;
					vector_t _indicatorVertex;
					vector_t _indicatorAtlasPoint;
					vector_t _indicatorAtlasVertex;
					vector_t _indicatorRgba;

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
							_slot = _toolkit.get_Queue()
								.reserve();
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
