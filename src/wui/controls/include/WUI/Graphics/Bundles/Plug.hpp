#ifndef WUI_GRAPHICS_BUNDLES_PLUG_HPP
#define WUI_GRAPHICS_BUNDLES_PLUG_HPP

#include <bitset>
#include <optional>

#include "math.hpp"

#include "WUI/Layouts/Atlas.hpp"

namespace WUI {
	namespace Graphics {
		namespace Bundles {
			template<typename TToolkit>
			class Plug {
			public:
				inline explicit Plug(TToolkit &toolkit)
					: _toolkit{ toolkit }
					, _slot{}
					, _changes{}
					, _framePoint{ 0, 0 }
					, _frameVertex{ 0, 0 }
					, _frameAtlasPoint{
						Layouts::Atlas::Plug::Frame::X
						+ Layouts::Atlas::Plug::Frame::Offset::Width,
						Layouts::Atlas::Plug::Frame::Y
						+ Layouts::Atlas::Plug::Frame::Offset::Height
					}
					, _frameAtlasVertex{
						Layouts::Atlas::Plug::Frame::Width,
						Layouts::Atlas::Plug::Frame::Height
					}
					, _frameRgba{ 0, 0, 0, 0 }
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
				Plug() = delete;
				Plug(Plug const &) = delete;
				Plug(Plug &&) = delete;

				inline ~Plug() = default;

				Plug & operator=(Plug const &) = delete;
				Plug & operator=(Plug &&) = delete;

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
						sub(&_labelVertex, 1) = sub(_frameVertex, 1)
							* Layouts::Atlas::Label::Height * 12
							/ (Layouts::Atlas::Label::Capital * 19);
						sub(&_labelVertex, 0) = 2 * sub(_labelVertex, 1);
						_changes.set(flags::framePoint);
					}
					if (_changes.test(flags::framePoint)) {
						sub(&_labelPoint, 0) = sub(_framePoint, 0)
							+ 7 * sub(_frameVertex, 0) / 41;
						sub(&_labelPoint, 1) = sub(_framePoint, 1)
							+ 7 * sub(_frameVertex, 1) / 38
							- (
									Layouts::Atlas::Label::Height
									- Layouts::Atlas::Label::Descender
									- Layouts::Atlas::Label::Capital
							) * sub(_labelVertex, 1) / Layouts::Atlas::Label::Height;
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
					label = 3,
					labelRgba = 4,
					renderTarget = 5
				};

				TToolkit &_toolkit;
				::std::optional<slot_t> _slot;
				::std::bitset<flags::renderTarget + 1> _changes;
				vec_t _framePoint;
				vec_t _frameVertex;
				vec_t _frameAtlasPoint;
				vec_t _frameAtlasVertex;
				vec_t _frameRgba;
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
