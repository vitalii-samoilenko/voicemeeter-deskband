#pragma once

#include <string>
#include <unordered_map>

#include <windows.h>
#include <wrl/client.h>
#include <d2d1_3.h>
#include <dwrite_3.h>

#include "estd/linear_algebra.h"

#include "Voicemeeter.DeskBand.UI/Graphics/ICanvas.h"

using namespace ::Voicemeeter::DeskBand::UI::Graphics;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace D2D {
				namespace Graphics {
					class Canvas final : public ICanvas {
					public:
						explicit Canvas(
							HWND hWnd
						);
						Canvas() = delete;
						Canvas(const Canvas&) = delete;
						Canvas(Canvas&&) = delete;

						~Canvas() = default;

						Canvas& operator=(const Canvas&) = delete;
						Canvas& operator=(Canvas&&) = delete;

						virtual const ::linear_algebra::vectord& get_Position() const override;
						virtual const ::linear_algebra::vectord& get_Size() const override;

						virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override;
						virtual void Resize(const ::linear_algebra::vectord& vertex) override;

						inline ID2D1HwndRenderTarget* get_pRenderTarget() const {
							return m_pD2dRenderTarget.Get();
						};
						bool get_pBrush(const ::std::string& key, ID2D1SolidColorBrush** ppBrush) const;
						bool get_pGeometry(const ::std::string& key, ID2D1PathGeometry** ppGeometry) const;
						inline IDWriteTextFormat* get_pTextFormat() const {
							return m_pTextFormat.Get();
						}


					private:
						HWND m_hWnd;
						::linear_algebra::vectord m_position;
						::linear_algebra::vectord m_vertex;
						::Microsoft::WRL::ComPtr<IDWriteFactory7> m_pDwFactory;
						::Microsoft::WRL::ComPtr<ID2D1Factory7> m_pD2dFactory;
						::Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_pD2dRenderTarget;
						::Microsoft::WRL::ComPtr<ID2D1GdiInteropRenderTarget> m_pD2dGdiRenderTarget;
						mutable ::std::unordered_map<::std::string, ::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> m_cpBrush;
						mutable ::std::unordered_map<::std::string, ::Microsoft::WRL::ComPtr<ID2D1PathGeometry>> m_cpGeometry;
						::Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTextFormat;
					};
				}
			}
		}
	}
}