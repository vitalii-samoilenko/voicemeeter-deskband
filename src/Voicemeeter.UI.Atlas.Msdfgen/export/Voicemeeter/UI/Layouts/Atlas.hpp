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
				namespace Label {
					constexpr num_t X{ push(0) };
					constexpr num_t Y{ push(52) };
					constexpr size_t Stride{ 3 };
					constexpr size_t Names{ 6 };
					constexpr num_t Width{ push(84) };
					constexpr num_t Height{ push(42) };
					namespace Offset{
						constexpr num_t Width{ push(3) };
						constexpr num_t Height{ push(3) };
					}
#ifdef WHEEL_EXTENDED_PRECISION
					constexpr num_t Descender{ push(79667832167832) / 10000000000000 };
					constexpr num_t Capital{ push(28457360820997) / 1000000000000 };
#else
					constexpr num_t Descender{ push(7966783) / 1000000 };
					constexpr num_t Capital{ push(2845736) / 100000 };
#endif
				}
				constexpr num_t Width{ push(270) };
				constexpr num_t Height{ push(1300) };
				namespace Knob {
					namespace Frame {
						constexpr num_t X{ push(0) };
						constexpr num_t Y{ push(0) };
						namespace Offset{
							constexpr num_t Width{ push(2) };
							constexpr num_t Height{ push(2) };
						}
						constexpr num_t Width{ push(48) }; // 40
						constexpr num_t Height{ push(48) }; // 40
					}
					namespace Indicator{
						constexpr num_t X{ push(52) };
						constexpr num_t Y{ push(21) };
						namespace Offset{
							constexpr num_t Width{ push(4) };
							constexpr num_t Height{ push(4) };
						}
						constexpr num_t Width{ push(11 * 4 / 2) }; // 5.5
						constexpr num_t Height{ push(11 * 4 / 2) }; // 5.5
					}
				}
				namespace Plug {
					namespace Frame {
						constexpr num_t X{ push(52) };
						constexpr num_t Y{ push(0) };
						namespace Offset{
							constexpr num_t Width{ push(1) };
							constexpr num_t Height{ push(1) };
						}
						constexpr num_t Width{ push(41) };
						constexpr num_t Height{ push(19) };
					}
				}
				namespace Vban {
					namespace Frame {
						constexpr num_t X{ push(95) };
						constexpr num_t Y{ push(0) };
						namespace Offset{
							constexpr num_t Width{ push(2) };
							constexpr num_t Height{ push(2) };
						}
						constexpr num_t Width{ push(39 * 2) }; // 70.(90)
						constexpr num_t Height{ push(22 * 2) }; // 40
					}
				}
			}
		}
	}
}

#endif
