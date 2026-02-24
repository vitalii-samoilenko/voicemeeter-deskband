#ifndef VOICEMEETER_UI_GRAPHICS_BUNDLES_VBAN_HPP
#define VOICEMEETER_UI_GRAPHICS_BUNDLES_VBAN_HPP

#include <bitset>
#include <optional>

#include "wheel.hpp"

#include "Voicemeeter/UI/Layouts/Atlas.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			namespace Bundles {
				template<typename TToolkit>
				class Vban {
				public:
					inline explicit Vban(TToolkit &toolkit)
						: _toolkit{ toolkit }
						, _slot{}
						, _changes{}
						, _framePoint{ 0, 0 }
						, _frameVertex{ 0, 0 }
						, _frameAtlasPoint{
							Layouts::Atlas::Vban::Frame::X
							+ Layouts::Atlas::Vban::Frame::Offset::Width,
							Layouts::Atlas::Vban::Frame::Y
							+ Layouts::Atlas::Vban::Frame::Offset::Height
						}
						, _frameAtlasVertex{
							Layouts::Atlas::Vban::Frame::Width,
							Layouts::Atlas::Vban::Frame::Height
						}
						, _frameRgba{ 0, 0, 0, 0 } {

					};
					Vban() = delete;
					Vban(Vban const &) = delete;
					Vban(Vban &&) = delete;

					inline ~Vban() = default;

					Vban & operator=(Vban const &) = delete;
					Vban & operator=(Vban &&) = delete;

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
						if (are_all(_framePoint == value)) {
							return;
						}
						_framePoint = value;
						OnInvalidate(flags::framePoint);
					};
					inline void set_FrameSize(vector_t const &value) {
						if (are_all(_frameVertex == value)) {
							return;
						}
						_frameVertex = value;
						OnInvalidate(flags::frameVertex);
					};
					inline void set_FrameColor(vector_t const &value) {
						if (are_all(_frameRgba == value)) {
							return;
						}
						_frameRgba = value;
						OnInvalidate(flags::frameRgba);
					};
					inline void set_Invalid() {
						OnInvalidate(flags::renderTarget);
					};

					inline void operator()() {
						if (_changes.test(flags::frameVertex)) {
							_changes.set(flags::framePoint);
						}
						if (_changes.test(flags::framePoint)) {
						}
						_changes.reset();
						_slot.reset();
						_toolkit.get_Frame()
							.Clear(_framePoint, _frameVertex);
						_toolkit.get_Atlas()
							.FillSDF(
								_frameAtlasPoint, _frameAtlasVertex,
								_framePoint, _frameVertex,
								_frameRgba);
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
						renderTarget = 3
					};

					TToolkit &_toolkit;
					::std::optional<slot_t> _slot;
					::std::bitset<flags::renderTarget + 1> _changes;
					vector_t _framePoint;
					vector_t _frameVertex;
					vector_t _frameAtlasPoint;
					vector_t _frameAtlasVertex;
					vector_t _frameRgba;

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
