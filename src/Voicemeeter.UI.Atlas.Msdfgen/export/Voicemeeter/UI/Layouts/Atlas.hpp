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
						constexpr num_t X{ push(0 + 1) };
						constexpr num_t Y{ push(0 + 1) };
						constexpr num_t Width{ push(48) };
						constexpr num_t Height{ push(48) };
					}
					namespace Indicator{
						constexpr num_t X{ push(50 + 1) };
						constexpr num_t Y{ push(22 + 1) };
						constexpr num_t Width{ push(11 * 4 / 2) };
						constexpr num_t Height{ push(11 * 4 / 2) };
					}
					namespace Label {
						constexpr num_t X{ 0 };
						constexpr num_t Y{ 4 };
					}
				}
				namespace Plug {
					namespace Frame {
						constexpr num_t X{ push(50 + 1) };
						constexpr num_t Y{ push(0 + 1) };
						constexpr num_t Width{ push(41) };
						constexpr num_t Height{ push(19) };
					}
					namespace Label {
						constexpr num_t X{ 0 };
						constexpr num_t Y{ 3 };
					}
				}
				namespace Vban {
					namespace Frame {
						constexpr num_t X{ push(93 + 1) };
						constexpr num_t Y{ push(0 + 1) };
						constexpr num_t Width{ push(39 * 2) };
						constexpr num_t Height{ push(22 * 2) };
					}
				}
			}
		}
	}
}

#endif
