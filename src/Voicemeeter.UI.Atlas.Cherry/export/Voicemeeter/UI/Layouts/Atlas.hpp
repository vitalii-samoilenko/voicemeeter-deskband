#ifndef VOICEMEETER_UI_LAYOUTS_ATLAS
#define VOICEMEETER_UI_LAYOUTS_ATLAS

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Layouts {
			namespace Atlas {
				constexpr num_t Factor{ 2 };
				namespace Block {
					constexpr num_t Width{ Factor * push(26) };
					constexpr num_t Height{ Factor * push(17) };
				}
				constexpr num_t Bidth{ 8 };
				constexpr num_t Beight{ 14 };
				constexpr num_t Width{ Bidth * Block::Width + One };
				constexpr num_t Height{ Beight * Block::Height + One };
				namespace Knob {
					namespace Frame {
						constexpr num_t X{ 4 * Block::Width + One };
						constexpr num_t Y{ 0 * Block::Height + One };
						constexpr num_t Width{ Factor * push(48) };
						constexpr num_t Height{ Factor * push(48) };
					}
					namespace Indicator{
						constexpr num_t X{ 6 * Block::Width + One };
						constexpr num_t Y{ 0 * Block::Height + One };
						constexpr num_t Width{ Factor * push(11) / 2 };
						constexpr num_t Height{ Factor * push(11) / 2 };
					}
					namespace Label {
						constexpr num_t X{ 0 * Block::Width + One };
						constexpr num_t Y{ 4 * Block::Height + One };
					}
				}
				namespace Plug {
					namespace Frame {
						constexpr num_t X{ 6 * Block::Width + One };
						constexpr num_t Y{ 1 * Block::Height + One };
						constexpr num_t Width{ Factor * push(41 * 24) / 20 };
						constexpr num_t Height{ Factor * push(19 * 24) / 20 };
					}
					namespace Label {
						constexpr num_t X{ 0 * Block::Width + One };
						constexpr num_t Y{ 3 * Block::Height + One };
					}
				}
				namespace Vban {
					namespace Frame {
						constexpr num_t X{ 0 * Block::Width + One };
						constexpr num_t Y{ 0 * Block::Height + One };
						constexpr num_t Width{ Factor * push(39 * 48) / 22 };
						constexpr num_t Height{ Factor * push(22 * 48) / 22 };
					}
				}
			}
		}
	}
}

#endif
