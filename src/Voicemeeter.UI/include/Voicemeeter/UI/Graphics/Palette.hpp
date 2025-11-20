#ifndef VOICEMEETER_UI_GRAPHICS_PALETTE_HPP
#define VOICEMEETER_UI_GRAPHICS_PALETTE_HPP

#include <chrono>
#include <memory>
#include <utility>
#include <valarray>
#include <vector>

#include "Voicemeeter/UI/Adapters/IBundle.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			class Stopwatch final {
			public:
				inline Stopwatch()
					: _current{}
					, _previous{} {

				};
				Stopwatch(Stopwatch const &) = delete;
				Stopwatch(Stopwatch &&) = delete;

				inline ~Stopwatch() = delete;

				Stopwatch & operator=(Stopwatch const &) = delete;
				Stopwatch & operator=(Stopwatch &&) = delete;

			inline int get_Elapsed() const {
				return static_cast<int>(
					::std::chrono::duration_cast<
						::std::chrono::milliseconds>(
						_current - previous)
							::count);
			};

			inline void Lap() {
				::std::swap(_current, _previous);
				_current = ::std::chrono::steady_clock::now();
			};

			private:
				::std::chrono::steady_clock::time_point _current;
				::std::chrono::steady_clock::time_point _previous;
			};

			class Queue final {
			public:
				inline Queue()
					: _items{} {

				};
				Queue(Queue const &) = delete;
				inline Queue(Queue &&) = default;

				inline ~Queue() = default;

				Queue & operator=(Queue const &) = delete;
				Queue & operator=(Queue &&) = delete;

				inline bool empty() const {
					return _items.empty();
				};

				inline auto begin() {
					return _items.begin();
				};
				inline auto end() {
					return _items.end();
				};

				template<typename TBundle>
				inline Queue & operator+=(TBundle &item) {
					_items.emplace_back(::std::make_unique<
						Adapters::Bundle<TBundle>>(
						item));
				};

			private:
				::std::vector<::std::unique_ptr<Adapters::IBundle>> _items;
			};

			template<
				typename TInstrumentation,
				typename TAtlas,
				typename TTheme>
			class Palette {
			public:
				template<typename... Args>
				inline explicit Palette(Args &&...args)
					: _instrumentation{ ::std::forward<Args>(args)... }
					, _atlas{ _instrumentation }
					, _stopwatch{}
					, _queue{}
					, _theme{} {

				};
				Palette() = delete;
				Palette(Palette const &) = delete;
				Palette(Palette &&) = delete;

				inline ~Palette() = default;

				Palette & operator=(Palette const &) = delete;
				Palette & operator=(Palette &&) = delete;

				inline TInstrumentation & get_Instrumentation() {
					return _instrumentation;
				};
				inline TAtlas & get_Atlas() {
					return _atlas;
				};
				inline Stopwatch & get_Stopwatch() {
					return _stopwatch;
				};
				inline Queue & get_Queue() {
					return _queue;
				};
				inline TTheme & get_Theme() {
					return _theme;
				};

				inline void Blend(
					::std::valarray<int> &dst,
					::std::valarray<int> const &src,
					int a) const {
					int temp{ dst[3] };
					dst = (dst * (SCALING_FACTOR - a) + src * a)
						/ SCALING_FACTOR;
					dst[3] = temp;
				};

			private:
				TInstrumentation _instrumentation;
				TAtlas _atlas;
				Stopwatch _stopwatch;
				Queue _queue;
				TTheme _theme;
			};
		}
	}
}

#endif
