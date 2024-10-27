#pragma once

#include <string>

#include <d2d1_3.h>

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace D2D {
				namespace Graphics {
					struct Theme {
						::std::wstring FontFamily;

						::D2D1::ColorF Danger;
						::D2D1::ColorF DarkGlass;
						::D2D1::ColorF LightGlass;
						::D2D1::ColorF PrimaryActive;
						::D2D1::ColorF SecondaryActive;
						::D2D1::ColorF Inactive;
						::D2D1::ColorF Indicator;

						static Theme Default() {
							return {
								L"Arial",
								::D2D1::ColorF{ 248 / 255.F, 99 / 255.F, 77 / 255.F },
								::D2D1::ColorF{ 0.75F, 0.75F, 0.75F, 0.4F },
								::D2D1::ColorF{ 1.F, 1.F, 1.F, 0.4F },
								::D2D1::ColorF{ 112 / 255.F, 195 / 255.F, 153 / 255.F },
								::D2D1::ColorF{ 104 / 255.F, 230 / 255.F, 250 / 255.F },
								::D2D1::ColorF{ 95 / 255.F, 120 / 255.F, 137 / 255.F },
								::D2D1::ColorF{ 1., 1., 1. }
							};
						};
					};
				}
			}
		}
	}
}