#ifndef VOICEMEETER_UI_GRAPHICS_BUNDLES_KNOB_HPP
#define VOICEMEETER_UI_GRAPHICS_BUNDLES_KNOB_HPP

#include <bitset>
#include <optional>

#include "wheel.hpp"

#include "Voicemeeter/UI/Layouts/Atlas.hpp"

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
						, _frameAtlasPoint{
							Layouts::Atlas::Knob::Frame::X,
							Layouts::Atlas::Knob::Frame::Y
						}
						, _frameAtlasVertex{
							Layouts::Atlas::Knob::Frame::Width,
							Layouts::Atlas::Knob::Frame::Height
						}
						, _frameRgba{ 0, 0, 0, 0 }
						, _indicatorDegree{}
						, _indicatorPoint{ 0, 0 }
						, _indicatorVertex{ 0, 0 }
						, _indicatorAtlasPoint{
							Layouts::Atlas::Knob::Indicator::X,
							Layouts::Atlas::Knob::Indicator::Y
						}
						, _indicatorAtlasVertex{
							Layouts::Atlas::Knob::Indicator::Width,
							Layouts::Atlas::Knob::Indicator::Height
						}
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
						OnSetV(_framePoint, point, flags::framePoint);
					};
					inline void set_FrameSize(vector_t const &vertex) {
						OnSetV(_frameVertex, vertex, flags::frameVertex);
					};
					inline void set_FrameColor(vector_t const &rgba) {
						OnSetV(_frameRgba, rgba, flags::frameRgba);
					};
					inline void set_IndicatorAngle(num_t degree) {
						OnSet(_indicatorDegree, degree, flags::indicatorDegree);
					};
					inline void set_IndicatorColor(vector_t const &rgba) {
						OnSetV(_indicatorRgba, rgba, flags::indicatorRgba);
					};
					inline void set_Invalid() {
						OnInvalidate(flags::renderTarget);
					};

					inline void operator()() {
						if (_changes.test(flags::frameVertex)) {
							_indicatorVertex = _frameVertex / 10;
							_changes.set(flags::framePoint);
						}
						if (_changes.test(flags::framePoint)) {
							_changes.set(flags::indicatorDegree);
						}
						if (_changes.test(flags::indicatorDegree)) {
							vector_t transformI{
								sinI(_indicatorDegree),
								cosI(_indicatorDegree)
							};
							auto frameR = _frameVertex / 2;
							auto center = _framePoint + frameR;
							auto indicatorR = frameR * 4 / 5;
							_indicatorPoint = center
								+ indicatorR / transformI
								- _indicatorVertex / 2;
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
								_indicatorRgba,
								true);
						_toolkit.get_Frame()
							.Invalidate(_framePoint, _frameVertex);
					};

				protected:
					using slot_t = typename TToolkit::Queue::slot;

					inline slot_t & get_Slot() {
						return _slot;
					};

				private:
					enum flags : size_t {
						framePoint = 0,
						frameVertex = 1,
						frameRgba = 2,
						indicatorDegree = 3,
						indicatorRgba = 4,
						renderTarget = 5
					};

					TToolkit &_toolkit;
					::std::optional<slot_t> _slot;
					::std::bitset<flags::renderTarget + 1> _changes;
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
					inline void OnSet(T &dst, T const &src, flags property) {
						if (src == dst) {
							return;
						}
						dst = src;
						OnInvalidate(property);
					};
					template<typename V>
					inline void OnSetV(V &dst, V const &src, flags property) {
						if (min(src == dst)) {
							return;
						}
						dst = src;
						OnInvalidate(property);
					};
					inline void OnInvalidate(flags property) {
						if (_changes.none()) {
							_slot = _toolkit.get_Queue()
								.reserve();
							_slot->overwrite(*this);
						}
						_changes.set(property);
					};
				};
			}
		}
	}
}

#endif
