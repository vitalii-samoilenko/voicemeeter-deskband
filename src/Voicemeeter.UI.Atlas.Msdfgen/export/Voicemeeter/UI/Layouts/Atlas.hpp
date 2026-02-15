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
				namespace Offset{
					constexpr num_t Width{ push(1) };
					constexpr num_t Height{ push(1) };
				}
				namespace Label {
					constexpr num_t X{ push(0) };
					constexpr num_t Y{ push(50) };
					constexpr num_t Width{ push(84) };
					constexpr num_t Height{ push(42) };
					constexpr num_t Stride{ 3 };
					constexpr num_t Descender{ push(796678) / 100000 };
					constexpr num_t Ascender{ Height - Descender };
					constexpr num_t Capital{ push(284574) / 10000 };
				}
				constexpr num_t Width{ push(258) };
				constexpr num_t Height{ push(1194) };
				namespace Knob {
					namespace Frame {
						constexpr num_t X{ push(0) };
						constexpr num_t Y{ push(0) };
						constexpr num_t Width{ push(48) };
						constexpr num_t Height{ push(48) };
					}
					namespace Indicator{
						constexpr num_t X{ push(50) };
						constexpr num_t Y{ push(22) };
						constexpr num_t Width{ push(11 * 4 / 2) };
						constexpr num_t Height{ push(11 * 4 / 2) };
					}
				}
				namespace Plug {
					namespace Frame {
						constexpr num_t X{ push(50) };
						constexpr num_t Y{ push(0) };
						constexpr num_t Width{ push(41) };
						constexpr num_t Height{ push(19) };
					}
				}
				namespace Vban {
					namespace Frame {
						constexpr num_t X{ push(93) };
						constexpr num_t Y{ push(0) };
						constexpr num_t Width{ push(39 * 2) };
						constexpr num_t Height{ push(22 * 2) };
					}
				}
			}
		}
	}
}

#endif
