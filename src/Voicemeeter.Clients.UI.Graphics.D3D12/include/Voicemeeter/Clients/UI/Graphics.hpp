#ifndef VOICEMEETER_CLIENTS_UI_GRAPHICS_HPP
#define VOICEMEETER_CLIENTS_UI_GRAPHICS_HPP

#include <exception>
#include <memory>

#include "Windows/API.hpp"

#include "Voicemeeter/UI/Adapters/Canvas.hpp"
#include "Voicemeeter/UI/Graphics/State.hpp"
#include "Voicemeeter/UI/Graphics/Atlas.hpp"
#include "Voicemeeter/UI/Graphics/Queue.hpp"
#include "Voicemeeter/UI/Graphics/Stopwatch.hpp"
#include "Voicemeeter/UI/Graphics/Frame.hpp"
#include "Voicemeeter/UI/Graphics/Toolkit.hpp"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			template<
				typename TPalette,
				typename TTheme,
				typename TTimer>
			using Graphics = UI::Adapters::Canvas<
				UI::Graphics::Toolkit<
					UI::Graphics::State,
					UI::Graphics::Atlas<
						UI::Graphics::State>,
					UI::Graphics::Queue,
					UI::Graphics::Stopwatch,
					TPalette,
					TTheme
					UI::Graphics::Frame<
						UI::Graphics::State,
						UI::Graphics::Queue,
						UI::Graphics::Stopwatch>>,
				TTimer>;

			template<
				typename TPalette,
				typename TTheme,
				typename TTimer>
			class GraphicsBuilder {
			public:
				using Graphics = Graphics<TPalette, TTheme, TTimer>;

				GraphicsBuilder(GraphicsBuilder const &) = delete;
				GraphicsBuilder(GraphicsBuilder &&) = delete;

				inline ~GraphicsBuilder() = default;

				GraphicsBuilder & operator=(GraphicsBuilder const &) = delete;
				GraphicsBuilder & operator=(GraphicsBuilder &&) = delete;

				inline void set_hWnd(HWND value) {
					_hWnd = value;
				};
				inline void set_hModule(HMODULE value) {
					_hModule = value;
				};
				inline void set_Timer(TTimer &value) {
					_timer = &value;
				};

			protected:
				inline GraphicsBuilder() = default;

				inline ::std::unique_ptr<Graphics> Build(
					::std::unique_ptr<TPalette> &&palette,
					::std::unique_ptr<TTheme> &&theme) {
					if (_hWnd == NULL) {
						throw ::std::exception{ "HWND is not set" };
					}
					if (!_timer) {
						throw ::std::exception{ "Timer is not set" };
					}
					::std::unique_ptr<Graphics::State> state{
						::std::make_unique<Graphics::State>(
							_hWnd, _hModule)
					};
					::std::unique_ptr<Graphics::Atlas> atlas{
						::std::make_unique<Graphics::Atlas>(
							*state)
					};
					::std::unique_ptr<Graphics::Queue> queue{
						::std::make_unique<Graphics::Queue>()
					};
					::std::unique_ptr<Graphics::Stopwatch> stopwatch{
						::std::make_unique<Graphics::Stopwatch>()
					};
					::std::unique_ptr<Graphics::Frame> frame{
						::std::make_unique<Graphics::Frame>(
							*state, *queue, *stopwatch)
					};
					return ::std::make_unique<Graphics>(
						*_timer,
						::std::move(state),
						::std::move(atlas),
						::std::move(queue),
						::std::move(stopwatch),
						::std::move(palette),
						::std::move(theme),
						::std::move(frame));
				};

			private:
				HWND _hWnd;
				HMODULE _hModule;
				TTimer *_timer;
			};
		}
	}
}

#endif
