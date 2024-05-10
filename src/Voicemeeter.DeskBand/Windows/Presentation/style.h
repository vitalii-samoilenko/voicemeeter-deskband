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
							D2D1::ColorF(44 / 255.F, 61 / 255.f, 77 / 255.f),
							D2D1::ColorF(125 / 255.f, 156 / 255.f, 177 / 255.f),
							D2D1::ColorF(112 / 255.f, 195 / 255.f, 153 / 255.f)
						};
					}

					LPCWSTR lpszMainFontFamilyName;
					D2D1_COLOR_F Main;
					D2D1_COLOR_F Front;
					D2D1_COLOR_F Active;
				};
			}
		}
	}
}