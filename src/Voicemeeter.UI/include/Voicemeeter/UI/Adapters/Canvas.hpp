#ifndef VOICEMEETER_UI_ADAPTERS_CANVAS_HPP
#define VOICEMEETER_UI_ADAPTERS_CANVAS_HPP

#include <utility>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			template<
				typename TToolkit,
				typename TTimer>
			class Canvas : public TToolkit {
			public:
				template<typename... Args>
				inline explicit Canvas(
					TTimer &timer,
					Args &&...args)
					: TToolkit{ ::std::forward<Args>(args) }
					, _timer{ timer }
					, _frameTick{ *this }
					, _point{ 0, 0 } {

				};
				Canvas(Canvas const &) = delete;
				Canvas(Canvas &&) = delete;

				inline ~Canvas() = default;

				Canvas & operator=(Canvas const &) = delete;
				Canvas & operator=(Canvas &&) = delete;

				inline ::std::valarray<int> const & get_Position() const {
					return _point;
				};
				inline ::std::valarray<int> const & get_Size() const {
					return TToolkit::get_Frame()
						.get_Size();
				};

				inline void Redraw(::std::valarray<int> const &point, ::std::valarray<int> const &vertex) {
					TToolkit::get_Frame()
						.Present(point, vertex);
				};
				inline void Resize(::std::valarray<int> const &vertex) {
					TToolkit::get_Frame()
						.set_Size(vertex);
				};
				inline void Show() {
					_timer.Set(_frameTick);
				};
				inline void Hide() {
					_timer.Unset(_frameTick);
				};

			private:
				class FrameTick {
				public:
					inline explicit FrameTick(TToolkit &toolkit)
						: _toolkit{ toolkit } {

					};
					FrameTick() = delete;
					FrameTick(FrameTick const &) = delete;
					FrameTick(FrameTick &&) = delete;

					inline ~FrameTick() = default;

					FrameTick & operator=(FrameTick const &) = delete;
					FrameTick & operator=(FrameTick &&) = delete;

					inline void operator()() const {
						_toolkit.get_Frame()
							.Update();
					};

				private:
					TToolkit &_toolkit;
				};

				TTimer &_timer;
				FrameTick _frameTick;
				::std::valarray<int> _point;
			};
		}
	}
}

#endif
