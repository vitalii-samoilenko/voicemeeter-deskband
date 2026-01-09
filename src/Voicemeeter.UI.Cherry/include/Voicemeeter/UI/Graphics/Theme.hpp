#ifndef VOICEMEETER_UI_GRAPHICS_THEME_HPP
#define VOICEMEETER_UI_GRAPHICS_THEME_HPP

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			struct Theme {
				::std::valarray<int> Inactive{ 0, 0, 0, 0 };
				::std::valarray<int> Active{ 0, 0, 0, 0 };
				::std::valarray<int> Warning{ 0, 0, 0, 0 };
				::std::valarray<int> Error{ 0, 0, 0, 0 };
				::std::valarray<int> Ok{ 0, 0, 0, 0 };
				::std::valarray<int> Information{ 0, 0, 0, 0 };
				::std::valarray<int> Neutral{ 0, 0, 0, 0 };
				::std::valarray<int> EqLow{ 0, 0, 0, 0 };
				::std::valarray<int> EqNormal{ 0, 0, 0, 0 };
				::std::valarray<int> EqHigh{ 0, 0, 0, 0 };
			};
		}
	}
}

#endif
