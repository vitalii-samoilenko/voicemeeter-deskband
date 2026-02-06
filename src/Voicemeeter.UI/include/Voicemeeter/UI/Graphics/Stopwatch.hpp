#ifndef VOICEMEETER_UI_GRAPHICS_STOPWATCH_HPP
#define VOICEMEETER_UI_GRAPHICS_STOPWATCH_HPP

#include <chrono>

#include "wheel.hpp"

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

				inline ~Stopwatch() = default;

				Stopwatch & operator=(Stopwatch const &) = delete;
				Stopwatch & operator=(Stopwatch &&) = delete;

			inline num_t get_Elapsed() const {
				return static_cast<num_t>(
					::std::chrono::duration_cast<
						::std::chrono::milliseconds>(
						_current - _previous)
							.count());
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
