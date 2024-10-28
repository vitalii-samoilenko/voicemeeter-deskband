#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>

#include "estd/linear_algebra.h"

#include <wrl/client.h>
#include <d2d1_3.h>
#include <dwrite_3.h>

#include "Voicemeeter.UI/Graphics/ICanvas.h"

#include "Theme.h"

using namespace ::Voicemeeter::UI::Graphics;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				class Palette;

				class Canvas final : public ICanvas {
				public:
					Canvas(
						HWND hWnd,
						const Theme& theme
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

					inline IDWriteFactory7* get_pDwFactory() const {
						return m_pDwFactory.Get();
					};
					inline ID2D1Factory7* get_pD2dFactory() const {
						return m_pD2dFactory.Get();
					}
					inline ID2D1HwndRenderTarget* get_pRenderTarget() const {
						return m_pD2dRenderTarget.Get();
					};
					inline const Palette& get_Palette() const {
						return *m_pPalette;
					}

				private:
					HWND m_hWnd;
					::linear_algebra::vectord m_position;
					::linear_algebra::vectord m_vertex;
					::Microsoft::WRL::ComPtr<IDWriteFactory7> m_pDwFactory;
					::Microsoft::WRL::ComPtr<ID2D1Factory7> m_pD2dFactory;
					::Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_pD2dRenderTarget;
					::Microsoft::WRL::ComPtr<ID2D1GdiInteropRenderTarget> m_pD2dGdiRenderTarget;
					::std::unique_ptr<Palette> m_pPalette;
				};

				class Palette final {
				public:
					Palette(
						const Theme& theme,
						const Canvas& canvas
					);
					Palette() = delete;
					Palette(const Palette&) = delete;
					Palette(Palette&&) = delete;

					~Palette() = default;

					Palette& operator=(const Palette&) = delete;
					Palette& operator=(Palette&&) = delete;

					inline const Theme& get_Theme() const {
						return m_theme;
					};
					IDWriteTextFormat* get_pTextFormat(const ::std::wstring& fontFamily) const;
					IDWriteTextLayout* get_pTextLayout(const ::std::wstring& text, const ::std::wstring& fontFamily) const;
					ID2D1SolidColorBrush* get_pBrush(const ::D2D1::ColorF& color) const;
					ID2D1PathGeometry* get_pGeometry(const ::std::type_info& type, bool& fresh) const;

				private:
					Theme m_theme;
					const Canvas& m_canvas;
					mutable ::std::unordered_map<void*, ::Microsoft::WRL::ComPtr<IDWriteTextFormat>> m_cpTextFormat;
					mutable ::std::unordered_map<void*, ::Microsoft::WRL::ComPtr<IDWriteTextLayout>> m_cpTextLayout;
					mutable ::std::unordered_map<void*, ::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> m_cpBrush;
					mutable ::std::unordered_map<void*, ::Microsoft::WRL::ComPtr<ID2D1PathGeometry>> m_cpGeometry;
				};
			}
		}
	}
}