#ifndef VOICEMEETER_CLIENTS_UI_CANVAS_HPP
#define VOICEMEETER_CLIENTS_UI_CANVAS_HPP

#include <memory>
#include <stdexcept>

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
				typename TSurface,
				typename TLoader,
				typename TPalette,
				typename TTheme,
				typename TTimer>
			using Canvas = ::Voicemeeter::UI::Adapters::Canvas<
				::Voicemeeter::UI::Graphics::Toolkit<
					TLoader,
					::Voicemeeter::UI::Graphics::State<TSurface, TLoader>,
					::Voicemeeter::UI::Graphics::Atlas<
						::Voicemeeter::UI::Graphics::State<TSurface, TLoader>>,
					::Voicemeeter::UI::Graphics::Queue,
					::Voicemeeter::UI::Graphics::Stopwatch,
					TPalette,
					TTheme,
					::Voicemeeter::UI::Graphics::Frame<
						TSurface,
						::Voicemeeter::UI::Graphics::State<TSurface, TLoader>,
						::Voicemeeter::UI::Graphics::Queue,
						::Voicemeeter::UI::Graphics::Stopwatch>>,
				TTimer>;

			template<
				typename TSurface,
				typename TLoader,
				typename TPalette,
				typename TTheme,
				typename TTimer>
			class CanvasBuilder {
			public:
				using Canvas = Canvas<TSurface, TLoader, TPalette, TTheme, TTimer>;

				CanvasBuilder(CanvasBuilder const &) = delete;
				CanvasBuilder(CanvasBuilder &&) = delete;

				inline ~CanvasBuilder() = default;

				CanvasBuilder & operator=(CanvasBuilder const &) = delete;
				CanvasBuilder & operator=(CanvasBuilder &&) = delete;

				inline CanvasBuilder & set_Surface(TSurface &value) {
					_surface = &value;
					return *this;
				};
				inline CanvasBuilder & set_Timer(TTimer &value) {
					_timer = &value;
					return *this;
				};

			protected:
				inline CanvasBuilder() = default;

				inline ::std::unique_ptr<Canvas> Build(
					::std::unique_ptr<TLoader> &&loader,
					::std::unique_ptr<TPalette> &&palette,
					::std::unique_ptr<TTheme> &&theme) {
					if (!_surface) {
						throw ::std::runtime_error{ "Surface is not set" };
					}
					if (!_timer) {
						throw ::std::runtime_error{ "Timer is not set" };
					}
					auto state = ::std::make_unique<
						Canvas::State>(*_surface, *loader);
					auto atlas = ::std::make_unique<
						Canvas::Atlas>(*state);
					auto queue = ::std::make_unique<
						Canvas::Queue>();
					auto stopwatch = ::std::make_unique<
						Canvas::Stopwatch>();
					auto frame = ::std::make_unique<
						Canvas::Frame>(*_surface, *state, *queue, *stopwatch);
					return ::std::make_unique<
						Canvas>(*_timer,
						::std::move(loader),
						::std::move(state),
						::std::move(atlas),
						::std::move(queue),
						::std::move(stopwatch),
						::std::move(palette),
						::std::move(theme),
						::std::move(frame));
				};

			private:
				TSurface *_surface;
				TTimer *_timer;
			};
		}
	}
}

#endif
