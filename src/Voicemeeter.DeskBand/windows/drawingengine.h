#pragma once

#include <unordered_map>
#include <vector>

#include "dwrite.h"
#pragma comment(lib, "dwrite")
#include <atlbase.h>

#include "style.h"
#include "window.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			struct Palette {
				ID2D1Factory* pFactory{ nullptr };
				IDWriteFactory* pDwFactory{ nullptr };
				LPCWSTR lpszFontFamilyName{ NULL };
				CComPtr<ID2D1HwndRenderTarget> pTarget;
				CComPtr<ID2D1SolidColorBrush> pMain;
				CComPtr<ID2D1SolidColorBrush> pSub;
				CComPtr<ID2D1SolidColorBrush> pActive;
			};

			class DrawingEngine {
				Style m_style;
				CComPtr<ID2D1Factory> m_pFactory;
				CComPtr<IDWriteFactory> m_pDwFactory;
				std::unordered_map<Window*, Palette> m_palettes;

			public:
				inline const Palette& get_rPalette(Window& rWnd) {
					return m_palettes[&rWnd];
				};

				explicit DrawingEngine(
					Style style
				);
				DrawingEngine() = delete;
				DrawingEngine(const DrawingEngine&) = delete;
				DrawingEngine(DrawingEngine&&) = default;

				~DrawingEngine() = default;

				DrawingEngine& operator=(const DrawingEngine&) = delete;
				DrawingEngine& operator=(DrawingEngine&&) = default;

				const Palette& Initialize(Window& rWnd);
				const Palette& Resize(Window& rWnd);
			};
		}
	}
}