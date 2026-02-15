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
							Layouts::Atlas::Knob::Frame::X
							+ Layouts::Atlas::Offset::Width,
							Layouts::Atlas::Knob::Frame::Y
							+ Layouts::Atlas::Offset::Height
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
							Layouts::Atlas::Knob::Indicator::X
							+ Layouts::Atlas::Offset::Width,
							Layouts::Atlas::Knob::Indicator::Y
							+ Layouts::Atlas::Offset::Height
						}
						, _indicatorAtlasVertex{
							Layouts::Atlas::Knob::Indicator::Width,
							Layouts::Atlas::Knob::Indicator::Height
						}
						, _indicatorRgba{ 0, 0, 0, 0 }
						, _label{ 0 }
						, _labelPoint{ 0, 0 }
						, _labelVertex{ 0, 0 }
						, _labelAtlasPoint{
							Layouts::Atlas::Knob::Label::X
							+ Layouts::Atlas::Offset::Width,
							Layouts::Atlas::Knob::Label::Y
							+ Layouts::Atlas::Offset::Height
						}
						, _labelAtlasVertex{
							0,
							0
						}
						, _labelRgba{ 0, 0, 0, 0 } {

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

					inline void set_FramePosition(vector_t const &value) {
						if (all(_framePoint == value)) {
							return;
						}
						_framePoint = value;
						OnInvalidate(flags::framePoint);
					};
					inline void set_FrameSize(vector_t const &value) {
						if (all(_frameVertex == value)) {
							return;
						}
						_frameVertex = value;
						OnInvalidate(flags::frameVertex);
					};
					inline void set_FrameColor(vector_t const &value) {
						if (all(_frameRgba == value)) {
							return;
						}
						_frameRgba = value;
						OnInvalidate(flags::frameRgba);
					};
					inline void set_IndicatorAngle(num_t value) {
						if (_indicatorDegree == value) {
							return;
						}
						_indicatorDegree = value;
						OnInvalidate(flags::indicatorDegree);
					};
					inline void set_IndicatorColor(vector_t const &value) {
						if (all(_indicatorRgba == value)) {
							return;
						}
						_indicatorRgba = value;
						OnInvalidate(flags::indicatorRgba);
					};
					inline void set_Label(size_t value) {
						if (_label == value) {
							return;
						}
						_label = value;
						OnInvalidate(flags::label);
					};
					inline void set_LabelColor(vector_t const &value) {
						if (all(_labelRgba == value)) {
							return;
						}
						_labelRgba = value;
						OnInvalidate(flags::labelRgba);
					};
					inline void set_Invalid() {
						OnInvalidate(flags::renderTarget);
					};

					inline void operator()() {
						if (_changes.test(flags::frameVertex)) {
							_indicatorVertex = _frameVertex / 10;
							_labelVertex[0] = _frameVertex[0] / 2;
							_labelVertex[1] = _frameVertex[1] / 4;
							_changes.set(flags::framePoint);
						}
						if (_changes.test(flags::framePoint)) {
							_labelPoint[0] = _framePoint[0] + _frameVertex[0] / 4;
							_labelPoint[1] = _framePoint[1] + _frameVertex[1] / 3;
							_changes.set(flags::indicatorDegree);
						}
						if (_changes.test(flags::indicatorDegree)) {
							vector_t transformI{
								sinI(-_indicatorDegree + push(90)),
								cosI(-_indicatorDegree + push(90))
							};
							auto frameR = _frameVertex / 2;
							auto center = _framePoint + frameR;
							auto indicatorR = frameR * 4 / 5;
							_indicatorPoint = center
								+ push(indicatorR) / transformI
								- _indicatorVertex / 2;
						}
						if (_changes.test(flags::label)) {
							_labelAtlasPoint[0] = Layouts::Atlas::Knob::Label::X
								+ 0 * (_label % 1);
							_labelAtlasPoint[1] = Layouts::Atlas::Knob::Label::Y
								+ 0 * (_label / 1);
						}
						_changes.reset();
						_slot.reset();
						_toolkit.get_Atlas()
							.FillSDF(
								_frameAtlasPoint, _frameAtlasVertex,
								_framePoint, _frameVertex,
								_frameRgba, false);
						_toolkit.get_Atlas()
							.FillSDF(
								_indicatorAtlasPoint, _indicatorAtlasVertex,
								_indicatorPoint, _indicatorVertex,
								_indicatorRgba, true);
						/*
						_toolkit.get_Atlas()
							.FillSDF(
								_labelAtlasPoint, _labelAtlasVertex,
								_labelPoint, _labelVertex,
								_labelRgba, true);
						*/
						_toolkit.get_Frame()
							.Invalidate(_framePoint, _frameVertex);
					};

				protected:
					using slot_t = typename TToolkit::Queue::slot;

					inline slot_t & get_Slot() {
						return _slot.value();
					};

				private:
					enum flags : size_t {
						framePoint = 0,
						frameVertex = 1,
						frameRgba = 2,
						indicatorDegree = 3,
						indicatorRgba = 4,
						label = 5,
						labelRgba = 6,
						renderTarget = 7
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
					size_t _label;
					vector_t _labelPoint;
					vector_t _labelVertex;
					vector_t _labelAtlasPoint;
					vector_t _labelAtlasVertex;
					vector_t _labelRgba;

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
