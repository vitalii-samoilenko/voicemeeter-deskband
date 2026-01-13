#ifndef VOICEMEETER_UI_ADAPTERS_CANVAS_HPP
#define VOICEMEETER_UI_ADAPTERS_CANVAS_HPP

#include <utility>

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
					, _frameTick{ timer, *this }
					, _point{ 0, 0 } {

				};
				Canvas() = delete;
				Canvas(Canvas const &) = delete;
				Canvas(Canvas &&) = delete;

				inline ~Canvas() = default;

				Canvas & operator=(Canvas const &) = delete;
				Canvas & operator=(Canvas &&) = delete;

				inline vector_t const & get_Position() const {
					return _point;
				};
				inline vector_t const & get_Size() const {
					return TToolkit::get_Frame()
						.get_Size();
				};

				inline void Redraw(vector_t const &point, vector_t const &vertex) {
					TToolkit::get_Frame()
						.Present(point, vertex);
				};
				inline void Resize(vector_t const &vertex) {
					TToolkit::get_Frame()
						.set_Size(vertex);
				};
				inline void Show() {
					_frameTick.Set();
				};
				inline void Hide() {
					_frameTick.Unset();
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

					inline void Set() {
						_timer.Set(17, *this);
					};
					inline void Unset() {
						_timer.Unset(*this);
					};

				private:
					TTimer &_timer;
					TToolkit &_toolkit;
				};

				FrameTick _frameTick;
				vector_t _point;
			};
		}
	}
}

#endif
