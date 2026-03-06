#ifndef VOICEMEETER_CLIENTS_WUI_CANVAS_HPP
#define VOICEMEETER_CLIENTS_WUI_CANVAS_HPP

#include <memory>
#include <stdexcept>

#include "WUI/Adapters/Canvas.hpp"
#include "WUI/Graphics/State.hpp"
#include "WUI/Graphics/Atlas.hpp"
#include "WUI/Graphics/Queue.hpp"
#include "WUI/Graphics/Stopwatch.hpp"
#include "WUI/Graphics/DirectFrame.hpp"
#include "WUI/Graphics/CachedFrame.hpp"
#include "WUI/Graphics/Toolkit.hpp"

namespace Voicemeeter {
	namespace Clients {
		namespace WUI {
			template<
				typename TSurface,
				typename TLoader,
				typename TPalette,
				typename TTheme,
				typename TTimer>
			using DirectCanvas = ::WUI::Adapters::Canvas<
				::WUI::Graphics::Toolkit<
					TLoader,
					::WUI::Graphics::State<TSurface, TLoader>,
					::WUI::Graphics::Atlas<
						::WUI::Graphics::State<TSurface, TLoader>>,
					::WUI::Graphics::Queue,
					::WUI::Graphics::Stopwatch,
					TPalette,
					TTheme,
					::WUI::Graphics::DirectFrame<
						TSurface,
						::WUI::Graphics::State<TSurface, TLoader>,
						::WUI::Graphics::Queue,
						::WUI::Graphics::Stopwatch>>,
				TTimer>;
			template<
				typename TSurface,
				typename TLoader,
				typename TPalette,
				typename TTheme,
				typename TTimer>
			using CachedCanvas = ::WUI::Adapters::Canvas<
				::WUI::Graphics::Toolkit<
					TLoader,
					::WUI::Graphics::State<TSurface, TLoader>,
					::WUI::Graphics::Atlas<
						::WUI::Graphics::State<TSurface, TLoader>>,
					::WUI::Graphics::Queue,
					::WUI::Graphics::Stopwatch,
					TPalette,
					TTheme,
					::WUI::Graphics::CachedFrame<
						TSurface,
						::WUI::Graphics::State<TSurface, TLoader>,
						::WUI::Graphics::Queue,
						::WUI::Graphics::Stopwatch>>,
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

			protected:
				inline DirectCanvasBuilder()
					: _surface{ nullptr }
					, _timer{ nullptr }
					, _layers{ 0 } {

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
