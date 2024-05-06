#pragma once

#include "scene.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class CheckboxScene : public Scene {
				D2D1_ELLIPSE m_outer;
				D2D1_ELLIPSE m_inner;
				CComPtr<ID2D1PathGeometry> m_mark;
				LPCWSTR m_lpszLabel;
				UINT32 m_lblSize;
				D2D1_RECT_F m_lblBoundary;
				CComPtr<IDWriteTextFormat> m_pLblFormat;

			public:
				explicit CheckboxScene(
					LPCWSTR lpszLabel
				);
				CheckboxScene() = delete;
				CheckboxScene(const CheckboxScene&) = delete;
				CheckboxScene(CheckboxScene&&) = delete;

				~CheckboxScene() = default;

				CheckboxScene& operator=(const CheckboxScene&) = delete;
				CheckboxScene& operator=(CheckboxScene&&) = delete;

				virtual void Resize(const Palette& rPalette) override;

				HRESULT Draw(const Palette& rPalette, bool checked);
				bool Inside(FLOAT x, FLOAT y);
			};
		}
	}
}