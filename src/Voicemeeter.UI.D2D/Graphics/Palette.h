#pragma once

#include <d2d1_3.h>
#include <dcomp.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include "Voicemeeter.UI.Cherry/Graphics/Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				class Instrumentation final {
				public:
					explicit Instrumentation(
						HWND hWnd
					);
					Instrumentation() = delete;
					Instrumentation(const Instrumentation&) = delete;
					Instrumentation(Instrumentation&&) = delete;

					~Instrumentation() = default;

					Instrumentation& operator=(Instrumentation&&) = delete;
					Instrumentation& operator=(const Instrumentation&) = delete;

					inline ID2D1Factory7* get_pD2dFactory() const {
						return m_pD2dFactory.Get();
					};
					inline ID2D1DeviceContext5* get_pDeviceContext() const {
						return m_pDeviceContext.Get();
					};
					inline IDXGISwapChain1* get_pSwapChain() const {
						return m_pSwapChain.Get();
					};
					inline ID2D1SolidColorBrush* get_pBrush() const {
						return m_pBrush.Get();
					};
					inline ID2D1Bitmap1* get_pBitmap() const {
						return m_pBitmap.Get();
					};
					inline ID2D1Bitmap1** get_ppBitmap() {
						return &m_pBitmap;
					};

				private:
					::Microsoft::WRL::ComPtr<ID2D1Factory7> m_pD2dFactory;
					::Microsoft::WRL::ComPtr<ID2D1DeviceContext5> m_pDeviceContext;
					::Microsoft::WRL::ComPtr<IDXGISwapChain1> m_pSwapChain;
					::Microsoft::WRL::ComPtr<IDCompositionTarget> m_pCompositionTarget;
					::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;
					::Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_pBitmap;
				};

				using Atlas = Cherry::Graphics::Atlas<Instrumentation>;
				using Bundle = Cherry::Graphics::Bundle<Instrumentation>;
				using Queue = Cherry::Graphics::Queue<Instrumentation>;
				using Palette = Cherry::Graphics::Palette<Instrumentation>;
			}
		}
	}
}