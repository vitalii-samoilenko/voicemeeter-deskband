#ifndef VOICEMEETER_UI_GRAPHICS_BUNDLES_PLUG_HPP
#define VOICEMEETER_UI_GRAPHICS_BUNDLES_PLUG_HPP

#include <bitset>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			namespace Bundles {
				template<typename TToolkit>
				class Plug {
				public:
					inline explicit Plug(TToolkit &toolkit)
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
						, _frameRgba{ 0, 0, 0, 0 } {

					};
					Plug() = delete;
					Plug(Plug const &) = delete;
					Plug(Plug &&) = delete;

					inline ~Plug() = default;

					Plug & operator=(Plug const &) = delete;
					Plug & operator=(Plug &&) = delete;

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
					inline void set_Invalid() {
						OnInvalidate(RenderTarget);
					};

					inline void operator()() {
						if (_changes.test(FrameVertex)) {
							_changes.set(FramePoint);
						}
						if (_changes.test(FramePoint)) {
						}
						_changes.reset();
						_slot.reset();
						_toolkit.get_Atlas()
							.FillSDF(
								_frameAtlasPoint, _frameAtlasVertex,
								_framePoint, _frameVertex,
								_frameRgba);
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
						Text,
						TextRgba,
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
