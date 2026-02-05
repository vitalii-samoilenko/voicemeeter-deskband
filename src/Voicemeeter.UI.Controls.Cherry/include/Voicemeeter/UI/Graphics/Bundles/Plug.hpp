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
						, _frameRgba{ 0, 0, 0, 0 } {

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

					inline void set_FramePosition(vector_t const &point) {
						OnSet(_framePoint, point, flags::framePoint);
					};
					inline void set_FrameSize(vector_t const &vertex) {
						OnSet(_frameVertex, vertex, flags::frameVertex);
					};
					inline void set_FrameColor(vector_t const &rgba) {
						OnSet(_frameRgba, rgba, flags::frameRgba);
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
						return _slot;
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
					::std::bitset<RenderTarget + 1> _changes;
					vector_t _framePoint;
					vector_t _frameVertex;
					vector_t _frameAtlasPoint;
					vector_t _frameAtlasVertex;
					vector_t _frameRgba;

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
