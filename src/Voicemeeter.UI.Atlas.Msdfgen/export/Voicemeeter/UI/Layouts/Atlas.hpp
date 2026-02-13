#ifndef VOICEMEETER_UI_LAYOUTS_ATLAS
#define VOICEMEETER_UI_LAYOUTS_ATLAS

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Layouts {
			namespace Atlas {
				namespace Range {
					constexpr num_t Width{ 4 };
					constexpr num_t Height{ 4 };
				}
				constexpr num_t Width{ push(173) };
				constexpr num_t Height{ push(50) };
				namespace Knob {
					namespace Frame {
						constexpr num_t X{ 0 };
						constexpr num_t Y{ 0 };
						constexpr num_t Width{ push(1 + 48 + 1) };
						constexpr num_t Height{ push(1 + 48 + 1) };
					}
					namespace Indicator{
						constexpr num_t X{ push(50) };
						constexpr num_t Y{ push(22) };
						constexpr num_t Width{ push(1 + 11 * 4 / 2 + 1) };
						constexpr num_t Height{ push(1 + 11 * 4 / 2 + 1) };
					}
					namespace Label {
						constexpr num_t X{ 0 };
						constexpr num_t Y{ 4 };
					}
				}
				namespace Plug {
					namespace Frame {
						constexpr num_t X{ push(50) };
						constexpr num_t Y{ 0 };
						constexpr num_t Width{ push(1 + 41 + 1) };
						constexpr num_t Height{ push(1 + 19 + 1) };
					}
					namespace Label {
						constexpr num_t X{ 0 };
						constexpr num_t Y{ 3 };
					}
				}
				namespace Vban {
					namespace Frame {
						constexpr num_t X{ push(93) };
						constexpr num_t Y{ 0 };
						constexpr num_t Width{ push(1 + 39 * 2 + 1) };
						constexpr num_t Height{ push(1 + 22 * 2 + 1) };
					}
				}
			}
		}
	}
}

#endif
