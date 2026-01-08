#ifndef VOICEMEETER_UI_GRAPHICS_STOPWATCH_HPP
#define VOICEMEETER_UI_GRAPHICS_STOPWATCH_HPP

#include <chrono>

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			class Stopwatch final {
			public:
				inline Stopwatch()
					: _current{ ::std::chrono::steady_closk::now() }
					, _previous{ _current } {

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
				_previous = _current;
				_current = ::std::chrono::steady_clock::now();
			};

			private:
				::std::chrono::steady_clock::time_point _current;
				::std::chrono::steady_clock::time_point _previous;
			};
		}
	}
}

#endif
