#ifndef VOICEMEETER_UI_GRAPHICS_TOOLKIT_HPP
#define VOICEMEETER_UI_GRAPHICS_TOOLKIT_HPP

#include <memory>
#include <utility>

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			template<
				typename TLoader,
				typename TState,
				typename TAtlas,
				typename TQueue,
				typename TStopwatch,
				typename TPalette,
				typename TTheme,
				typename TFrame>
			class Toolkit {
			public:
				using Loader = TLoader;
				using State = TState;
				using Atlas = TAtlas;
				using Queue = TQueue;
				using Stopwatch = TStopwatch;
				using Palette = TPalette;
				using Theme = TTheme;
				using Frame = TFrame;

				inline Toolkit(
					::std::unique_ptr<Loader> &&loader,
					::std::unique_ptr<State> &&state,
					::std::unique_ptr<Atlas> &&atlas,
					::std::unique_ptr<Queue> &&queue,
					::std::unique_ptr<Stopwatch> &&stopwatch,
					::std::unique_ptr<Palette> &&palette,
					::std::unique_ptr<Theme> &&theme,
					::std::unique_ptr<Frame> &&frame)
					: _loader{ ::std::move(loader} }
					, _state{ ::std::move(state) }
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

				inline Loader & get_Loader() {
					return *_loader;
				};
				inline State & get_State() {
					return *_state;
				};
				inline Atlas & get_Atlas() {
					return *_atlas;
				};
				inline Queue & get_Queue() {
					return *_queue;
				};
				inline Stopwatch & get_Stopwatch() {
					return *_stopwatch;
				};
				inline Palette & get_Palette() {
					return *_palette;
				};
				inline Theme & get_Theme() {
					return *_theme;
				};
				inline Frame & get_Frame() {
					return *_frame;
				};

			private:
				::std::unique_ptr<Loader> _loader;
				::std::unique_ptr<State> _state;
				::std::unique_ptr<Atlas> _atlas;
				::std::unique_ptr<Queue> _queue;
				::std::unique_ptr<Stopwatch> _stopwatch;
				::std::unique_ptr<Palette> _palette;
				::std::unique_ptr<Theme> _theme;
				::std::unique_ptr<Frame> _frame;
			};
		}
	}
}

#endif
