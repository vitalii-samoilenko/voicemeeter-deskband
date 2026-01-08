#ifndef VOICEMEETER_UI_GRAPHICS_TOOLKIT_HPP
#define VOICEMEETER_UI_GRAPHICS_TOOLKIT_HPP

#include <memory>
#include <utility>

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			template<
				typename TState,
				typename TAtlas,
				typename TQueue,
				typename TStopwatch,
				typename TPalette,
				typename TTheme,
				typename TFrame>
			class Toolkit {
			public:
				inline Toolkit(
					::std::unique_ptr<TState> &&state,
					::std::unique_ptr<TAtlas> &&atlas,
					::std::unique_ptr<TQueue> &&queue,
					::std::unique_ptr<TStopwatch> &&stopwatch,
					::std::unique_ptr<TPalette> &&palette,
					::std::unique_ptr<TTheme> &&theme,
					::std::unique_ptr<TFrame> &&frame)
					: _state{ ::std::move(state) }
					, _atlas{ ::std::move(atlas) }
					, _queue{ ::std::move(queue) }
					, _stopwatch{ ::std::move(stopwatch) }
					, _palette{ ::std::move(palette) }
					, _theme{ ::std::move(theme) }
					, _frame{ ::std::move(frame) } {

				};
				Toolkit() = delete;
				Toolkit(Toolkit const &) = delete;
				Toolkit(Toolkit &&) = delete;

				inline ~Toolkit() = default;

				Toolkit & operator=(Toolkit const &) = delete;
				Toolkit & operator=(Toolkit &&) = delete;

				inline TState & get_State() {
					return *_state;
				};
				inline TAtlas & get_Atlas() {
					return *_atlas;
				};
				inline TQueue & get_Queue() {
					return *_queue;
				};
				inline TStopwatch & get_Stopwatch() {
					return *_stopwatch;
				};
				inline TPalette & get_Palette() {
					return *_palette;
				};
				inline TTheme & get_Theme() {
					return *_theme;
				};
				inline TFrame & get_Frame() {
					return *_frame;
				};

			private:
				::std::unique_ptr<TState> _state;
				::std::unique_ptr<TAtlas> _atlas;
				::std::unique_ptr<TQueue> _queue;
				::std::unique_ptr<TStopwatch> _stopwatch;
				::std::unique_ptr<TPalette> _palette;
				::std::unique_ptr<TTheme> _theme;
				::std::unique_ptr<TFrame> _frame;
			};
		}
	}
}

#endif
