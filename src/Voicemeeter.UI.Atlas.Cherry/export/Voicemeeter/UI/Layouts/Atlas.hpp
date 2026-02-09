#ifndef VOICEMEETER_UI_LAYOUTS_ATLAS
#define VOICEMEETER_UI_LAYOUTS_ATLAS

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Layouts {
			namespace Atlas {
				namespace Block {
					constexpr num_t Width{ push(26) };
					constexpr num_t Height{ push(17) };
				}
				constexpr num_t Width{ 8 * Block::Width + One };
				constexpr num_t Height{ 14 * Block::Height + One };
				namespace Knob {
					namespace Frame {
						constexpr num_t X{ 4 * Block::Width + One };
						constexpr num_t Y{ 0 * Block::Height + One };
						constexpr num_t Width{ push(48) };
						constexpr num_t Height{ push(48) };
					}
					namespace Indicator{
						constexpr num_t X{ 6 * Block::Width + One };
						constexpr num_t Y{ 0 * Block::Height + One };
						constexpr num_t Width{ push(6) };
						constexpr num_t Height{ push(6) };
					}
				}
				namespace Plug {
					namespace Frame {
						constexpr num_t X{ 6 * Block::Width + One };
						constexpr num_t Y{ 1 * Block::Height + One };
						constexpr num_t Width{ push(41 * 24) / 20 };
						constexpr num_t Height{ push(19 * 24) / 20 };
					}
				}
				namespace Vban {
					namespace Frame {
						constexpr num_t X{ 0 * Block::Width + One };
						constexpr num_t Y{ 0 * Block::Height + One };
						constexpr num_t Width{ push(39 * 48) / 22 };
						constexpr num_t Height{ push(22 * 48) / 22 };
					}
				}
			}
		}
	}
}

#endif
