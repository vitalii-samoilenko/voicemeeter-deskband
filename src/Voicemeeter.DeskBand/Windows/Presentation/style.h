#pragma once

#include <d2d1_3.h>
#pragma comment(lib, "d2d1")

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				struct Style {
					static inline Style Default() {
						return Style{
							L"Arial",
							D2D1::ColorF(44 / 255.F, 61 / 255.F, 77 / 255.F, 1.F),
							D2D1::ColorF(125 / 255.F, 156 / 255.F, 177 / 255.F, 1.F),
							D2D1::ColorF(112 / 255.F, 195 / 255.F, 153 / 255.F, 1.F)
						};
					}

					const LPCWSTR lpszMainFontFamilyName;
					const D2D1_COLOR_F Main;
					const D2D1_COLOR_F Front;
					const D2D1_COLOR_F Active;
				};
			}
		}
	}
}