#ifndef VOICEMEETER_UI_GRAPHICS_BUNDLES_PLUG_HPP
#define VOICEMEETER_UI_GRAPHICS_BUNDLES_PLUG_HPP

#include <bitset>
#include <optional>

#include "wheel.hpp"

#include "Voicemeeter/UI/Layouts/Atlas.hpp"

namespace Voicemeeter {
	namespace UI {
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
							Layouts::Atlas::Plug::Frame::X,
							Layouts::Atlas::Plug::Frame::Y
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
							Layouts::Atlas::Plug::Label::X,
							Layouts::Atlas::Plug::Label::Y
						}
						, _labelAtlasVertex{
							Layouts::Atlas::Block::Width,
							Layouts::Atlas::Block::Height
						}
						, _labelRgba{ 0, 0, 0, 0 } {

					};
					Plug() = delete;
					Plug(Plug const &) = delete;
					Plug(Plug &&) = delete;

					inline ~Plug() = default;

					Plug & operator=(Plug const &) = delete;
					Plug & operator=(Plug &&) = delete;

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
							_labelVertex[0] = _frameVertex[0] / 2;
							_labelVertex[1] = _frameVertex[1] * 4 / 5;
							_changes.set(flags::framePoint);
						}
						if (_changes.test(flags::framePoint)) {
							_labelPoint[0] = _framePoint[0] + _frameVertex[0] * 3 / 10;
							_labelPoint[1] = _framePoint[1] + _frameVertex[1] / 10;
						}
						if (_changes.test(flags::label)) {
							_labelAtlasPoint[0] = Layouts::Atlas::Plug::Label::X
								+ Layouts::Atlas::Block::Width * (_label % Layouts::Atlas::Bidth);
							_labelAtlasPoint[1] = Layouts::Atlas::Plug::Label::Y
								+ Layouts::Atlas::Block::Height * (_label / Layouts::Atlas::Bidth);
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
								_labelAtlasPoint, _labelAtlasVertex,
								_labelPoint, _labelVertex,
								_labelRgba, true);
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
						label = 3,
						labelRgba = 4,
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
