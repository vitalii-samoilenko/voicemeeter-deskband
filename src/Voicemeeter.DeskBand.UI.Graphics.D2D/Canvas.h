#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <d2d1_3.h>
#include <dwrite_3.h>

#include "estd/linear_algebra.h"

#include "Voicemeeter.DeskBand.UI/Graphics/ICanvas.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Graphics {
				namespace D2D {
					class Canvas final : public ICanvas {
					public:
						explicit Canvas(
							HWND hWnd,
							::D2D1::ColorF background
						);
						Canvas() = delete;
						Canvas(const Canvas&) = delete;
						Canvas(Canvas&&) = delete;

						~Canvas() = default;

						Canvas& operator=(const Canvas&) = delete;
						Canvas& operator=(Canvas&&) = delete;

						virtual const ::linear_algebra::vector& get_Position() const override;
						virtual const ::linear_algebra::vector& get_Size() const override;

						virtual void Redraw(const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) override;
						virtual void Resize(const ::linear_algebra::vector& vertex) override;

						inline ID2D1HwndRenderTarget* get_pRenderTarget() const {
							return m_pD2dRenderTarget.Get();
						};
						inline ID2D1SolidColorBrush* get_pBackgroundBrush() const {
							return m_pBackgroundBrush.Get();
						};

					private:
						::Microsoft::WRL::ComPtr<IDWriteFactory7> m_pDwFactory;
						::Microsoft::WRL::ComPtr<ID2D1Factory7> m_pD2dFactory;
						::Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_pD2dRenderTarget;
						::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBackgroundBrush;
					};
				}
			}
		}
	}
}