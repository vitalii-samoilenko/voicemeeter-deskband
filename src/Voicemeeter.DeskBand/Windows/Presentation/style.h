#pragma once

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				using ColorF = FLOAT[4];

				struct Style {
					static inline Style Default() {
						return Style{
							L"Arial",
							{44 / 255.F, 61 / 255.F, 77 / 255.F, 1.F},
							{125 / 255.F, 156 / 255.F, 177 / 255.F, 1.F},
							{112 / 255.F, 195 / 255.F, 153 / 255.F, 1.F}
						};
					}

					const LPCWSTR lpszMainFontFamilyName;
					const ColorF Main;
					const ColorF Front;
					const ColorF Active;
				};
			}
		}
	}
}