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
#include "Voicemeeter/UI/Graphics/DirectFrame.hpp"
#include "Voicemeeter/UI/Graphics/CachedFrame.hpp"
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
			using DirectCanvas = ::Voicemeeter::UI::Adapters::Canvas<
				::Voicemeeter::UI::Graphics::Toolkit<
					TLoader,
					::Voicemeeter::UI::Graphics::State<TSurface, TLoader>,
					::Voicemeeter::UI::Graphics::Atlas<
						::Voicemeeter::UI::Graphics::State<TSurface, TLoader>>,
					::Voicemeeter::UI::Graphics::Queue,
					::Voicemeeter::UI::Graphics::Stopwatch,
					TPalette,
					TTheme,
					::Voicemeeter::UI::Graphics::DirectFrame<
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
			using CachedCanvas = ::Voicemeeter::UI::Adapters::Canvas<
				::Voicemeeter::UI::Graphics::Toolkit<
					TLoader,
					::Voicemeeter::UI::Graphics::State<TSurface, TLoader>,
					::Voicemeeter::UI::Graphics::Atlas<
						::Voicemeeter::UI::Graphics::State<TSurface, TLoader>>,
					::Voicemeeter::UI::Graphics::Queue,
					::Voicemeeter::UI::Graphics::Stopwatch,
					TPalette,
					TTheme,
					::Voicemeeter::UI::Graphics::CachedFrame<
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
			class DirectCanvasBuilder {
			public:
				using Canvas = DirectCanvas<TSurface, TLoader, TPalette, TTheme, TTimer>;

				DirectCanvasBuilder(DirectCanvasBuilder const &) = delete;
				DirectCanvasBuilder(DirectCanvasBuilder &&) = delete;

				inline ~DirectCanvasBuilder() = default;

				DirectCanvasBuilder & operator=(DirectCanvasBuilder const &) = delete;
				DirectCanvasBuilder & operator=(DirectCanvasBuilder &&) = delete;

				inline DirectCanvasBuilder & set_Surface(TSurface &value) {
					_surface = &value;
					return *this;
				};
				inline DirectCanvasBuilder & set_Timer(TTimer &value) {
					_timer = &value;
					return *this;
				};
				inline DirectCanvasBuilder & set_Layers(size_t value) {
					_layers = value;
					return *this;
				};

			protected:
				inline DirectCanvasBuilder()
					: _surface{ nullptr }
					, _timer{ nullptr }
					, _layers{ 1 } {

				};

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
						Canvas::State>(*_surface, *loader, _layers);
					auto atlas = ::std::make_unique<
						Canvas::Atlas>(*state);
					auto queue = ::std::make_unique<
						Canvas::Queue>();
					auto stopwatch = ::std::make_unique<
						Canvas::Stopwatch>();
					auto frame = ::std::make_unique<
						Canvas::Frame>(
						*_surface, *state, *queue, *stopwatch);
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
				size_t _layers;
			};
			template<
				typename TSurface,
				typename TLoader,
				typename TPalette,
				typename TTheme,
				typename TTimer>
			class CachedCanvasBuilder {
			public:
				using Canvas = CachedCanvas<TSurface, TLoader, TPalette, TTheme, TTimer>;

				CachedCanvasBuilder(CachedCanvasBuilder const &) = delete;
				CachedCanvasBuilder(CachedCanvasBuilder &&) = delete;

				inline ~CachedCanvasBuilder() = default;

				CachedCanvasBuilder & operator=(CachedCanvasBuilder const &) = delete;
				CachedCanvasBuilder & operator=(CachedCanvasBuilder &&) = delete;

				inline CachedCanvasBuilder & set_Surface(TSurface &value) {
					_surface = &value;
					return *this;
				};
				inline CachedCanvasBuilder & set_Timer(TTimer &value) {
					_timer = &value;
					return *this;
				};
				inline CachedCanvasBuilder & set_Layers(size_t value) {
					_layers = value;
					return *this;
				};

			protected:
				inline CachedCanvasBuilder()
					: _surface{ nullptr }
					, _timer{ nullptr }
					, _layers{ 1 } {

				};

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
						Canvas::State>(*_surface, *loader, _layers);
					auto atlas = ::std::make_unique<
						Canvas::Atlas>(*state);
					auto queue = ::std::make_unique<
						Canvas::Queue>();
					auto stopwatch = ::std::make_unique<
						Canvas::Stopwatch>();
					auto frame = ::std::make_unique<
						Canvas::Frame>(
						*_surface, *state, *queue, *stopwatch);
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
				size_t _layers;
			};
		}
	}
}

#endif
