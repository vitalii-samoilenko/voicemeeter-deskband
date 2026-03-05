#ifndef WUI_GRAPHICS_BUNDLES_KNOB_HPP
#define WUI_GRAPHICS_BUNDLES_KNOB_HPP

#include <bitset>
#include <optional>

#include "math.hpp"

#include "WUI/Layouts/Atlas.hpp"

namespace WUI {
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
						+ Layouts::Atlas::Knob::Frame::Offset::Width,
						Layouts::Atlas::Knob::Frame::Y
						+ Layouts::Atlas::Knob::Frame::Offset::Height
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
						+ Layouts::Atlas::Knob::Indicator::Offset::Width,
						Layouts::Atlas::Knob::Indicator::Y
						+ Layouts::Atlas::Knob::Indicator::Offset::Height
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
						Layouts::Atlas::Label::X
						+ Layouts::Atlas::Label::Offset::Width,
						Layouts::Atlas::Label::Y
						+ Layouts::Atlas::Label::Offset::Height
					}
					, _labelAtlasVertex{
						Layouts::Atlas::Label::Width,
						Layouts::Atlas::Label::Height
					}
					, _labelRgba{ 0, 0, 0, 0 } {

				};
				Knob() = delete;
				Knob(Knob const &) = delete;
				Knob(Knob &&) = delete;

				inline ~Knob() = default;

				Knob & operator=(Knob const &) = delete;
				Knob & operator=(Knob &&) = delete;

				inline vec_t const & get_FramePosition() const {
					return _framePoint;
				};
				inline vec_t const & get_FrameSize() const {
					return _frameVertex;
				};
				inline vec_t const & get_FrameColor() const {
					return _frameRgba;
				};

				inline void set_FramePosition(vec_t const &value) {
					if (are_all(_framePoint == value)) {
						return;
					}
					_framePoint = value;
					OnInvalidate(flags::framePoint);
				};
				inline void set_FrameSize(vec_t const &value) {
					if (are_all(_frameVertex == value)) {
						return;
					}
					_frameVertex = value;
					OnInvalidate(flags::frameVertex);
				};
				inline void set_FrameColor(vec_t const &value) {
					if (are_all(_frameRgba == value)) {
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
				inline void set_IndicatorColor(vec_t const &value) {
					if (are_all(_indicatorRgba == value)) {
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
				inline void set_LabelColor(vec_t const &value) {
					if (are_all(_labelRgba == value)) {
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
						_indicatorVertex = _frameVertex * 11 / 96;
						sub(&_labelVertex, 1) = sub(_frameVertex, 1)
							* Layouts::Atlas::Label::Height * 10
							/ (Layouts::Atlas::Label::Capital * 42);
						sub(&_labelVertex, 0) = 2 * sub(_labelVertex, 1);
						_changes.set(flags::framePoint);
					}
					if (_changes.test(flags::framePoint)) {
						sub(&_labelPoint, 0) = sub(_framePoint, 0)
							+ (sub(_frameVertex, 0) - sub(_labelVertex, 0)) / 2;
						sub(&_labelPoint, 1) = sub(_framePoint, 1)
							+ (
								sub(_frameVertex, 1) * (
									Layouts::Atlas::Label::Capital * 26
									- (
										Layouts::Atlas::Label::Height
										- Layouts::Atlas::Label::Descender
									) * 10
								)
							) / (Layouts::Atlas::Label::Capital * 42);
						_changes.set(flags::indicatorDegree);
					}
					if (_changes.test(flags::indicatorDegree)) {
						_indicatorPoint = _framePoint
							+ (_frameVertex - _indicatorVertex) / 2
							+ (_frameVertex * push(31) / 96)
							/ vec_t{
								sinI(push(90) - _indicatorDegree),
								cosI(push(90) - _indicatorDegree)
							};
					}
					if (_changes.test(flags::label)) {
						sub(&_labelAtlasPoint, 0) = Layouts::Atlas::Label::X
							+ (
									Layouts::Atlas::Label::Offset::Width
									+ Layouts::Atlas::Label::Width
									+ Layouts::Atlas::Label::Offset::Width
								) * (_label % Layouts::Atlas::Label::Stride)
							+ Layouts::Atlas::Label::Offset::Width;
						sub(&_labelAtlasPoint, 1) = Layouts::Atlas::Label::Y
							+ (
									Layouts::Atlas::Label::Offset::Height
									+ Layouts::Atlas::Label::Height
									+ Layouts::Atlas::Label::Offset::Height
								) * (_label / Layouts::Atlas::Label::Stride)
							+ Layouts::Atlas::Label::Offset::Height;
					}
					_changes.reset();
					_slot.reset();
					_toolkit.get_Frame()
						.Prepare(_framePoint, _frameVertex);
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
					_toolkit.get_Atlas()
						.FillSDF(
							_labelAtlasPoint, _labelAtlasVertex,
							_labelPoint, _labelVertex,
							_labelRgba);
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
				vec_t _framePoint;
				vec_t _frameVertex;
				vec_t _frameAtlasPoint;
				vec_t _frameAtlasVertex;
				vec_t _frameRgba;
				num_t _indicatorDegree;
				vec_t _indicatorPoint;
				vec_t _indicatorVertex;
				vec_t _indicatorAtlasPoint;
				vec_t _indicatorAtlasVertex;
				vec_t _indicatorRgba;
				size_t _label;
				vec_t _labelPoint;
				vec_t _labelVertex;
				vec_t _labelAtlasPoint;
				vec_t _labelAtlasVertex;
				vec_t _labelRgba;

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

#endif
