#ifndef VOICEMEETER_UI_GRAPHICS_THEME_HPP
#define VOICEMEETER_UI_GRAPHICS_THEME_HPP

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			struct Theme {
				vec_t Inactive{ push(95), push(120), push(137), push(255) };
				vec_t Active{ push(112), push(195), push(153), push(255) };
				vec_t Warning{ push(231), push(255), push(17), push(255) };
				vec_t Error{ push(248), push(99), push(77), push(255) };
				vec_t Information{ push(104), push(230), push(250), push(255) };
				vec_t Neutral{ push(255), push(255), push(255), push(255) };
				vec_t EqLow{ push(139), push(179), push(187), push(255) };
				vec_t EqMedium{ push(30), push(254), push(91), push(255) };
				vec_t EqHigh{ push(255), push(0), push(0), push(255) };
			};
		}
	}
}

#endif
