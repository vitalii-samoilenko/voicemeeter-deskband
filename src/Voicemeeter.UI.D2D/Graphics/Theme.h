#pragma once

#include <string>

#include <d2d1_3.h>

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				struct Theme {
					::std::wstring FontFamily;
					::D2D1::ColorF Background;
					::D2D1::ColorF Warning;
					::D2D1::ColorF Danger;
					::D2D1::ColorF DarkGlass;
					::D2D1::ColorF LightGlass;
					::D2D1::ColorF PrimaryActive;
					::D2D1::ColorF SecondaryActive;
					::D2D1::ColorF Inactive;
					::D2D1::ColorF Indicator;
					::D2D1::ColorF EqualizerLow;
					::D2D1::ColorF EqualizerMedium;
					::D2D1::ColorF EqualizerHigh;

					static Theme Dark() {
						return {
							L"Arial",
							{ 0.F, 0.F, 0.F, 0.F },
							{ 231 / 255.F, 255 / 255.F, 17 / 255.F },
							{ 248 / 255.F, 99 / 255.F, 77 / 255.F },
							{ 0.75F, 0.75F, 0.75F, 0.4F },
							{ 1.F, 1.F, 1.F, 0.4F },
							{ 112 / 255.F, 195 / 255.F, 153 / 255.F },
							{ 104 / 255.F, 230 / 255.F, 250 / 255.F },
							{ 95 / 255.F, 120 / 255.F, 137 / 255.F },
							{ 1., 1., 1. },
							{ 139 / 255.F, 179 / 255.F, 187 / 255.F },
							{ 30 / 255.F, 254 / 255.F, 91 / 255.F },
							{ 1.F, 0.F, 0.F }
						};
					};
					static Theme Light() {
						return {
							L"Arial",
							{ 0.F, 0.F, 0.F, 0.F },
							{ 215 / 255.F, 215 / 255.F, 215 / 255.F },
							{ 248 / 255.F, 99 / 255.F, 77 / 255.F },
							{ 0.75F, 0.75F, 0.75F, 0.4F },
							{ 1.F, 1.F, 1.F, 0.4F },
							{ 112 / 255.F, 195 / 255.F, 153 / 255.F },
							{ 104 / 255.F, 230 / 255.F, 250 / 255.F },
							{ 95 / 255.F, 120 / 255.F, 137 / 255.F },
							{ 215 / 255.F, 215 / 255.F, 215 / 255.F },
							{ 139 / 255.F, 179 / 255.F, 187 / 255.F },
							{ 30 / 255.F, 254 / 255.F, 91 / 255.F },
							{ 1.F, 0.F, 0.F }
						};
					};
				};
			}
		}
	}
}