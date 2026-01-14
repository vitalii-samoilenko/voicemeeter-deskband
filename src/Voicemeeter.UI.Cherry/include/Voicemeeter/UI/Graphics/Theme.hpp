#ifndef VOICEMEETER_UI_GRAPHICS_THEME_HPP
#define VOICEMEETER_UI_GRAPHICS_THEME_HPP

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			struct Theme {
				vector_t Inactive{ 0, 0, 0, 0 };
				vector_t Active{ 0, 0, 0, 0 };
				vector_t Warning{ 0, 0, 0, 0 };
				vector_t Error{ 0, 0, 0, 0 };
				vector_t Ok{ 0, 0, 0, 0 };
				vector_t Information{ 0, 0, 0, 0 };
				vector_t Neutral{ 0, 0, 0, 0 };
				vector_t EqLow{ 0, 0, 0, 0 };
				vector_t EqNormal{ 0, 0, 0, 0 };
				vector_t EqHigh{ 0, 0, 0, 0 };
			};
		}
	}
}

#endif
