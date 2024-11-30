#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>

#include <d2d1_3.h>
#include <dcomp.h>
#include <dwrite_3.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include "estd/type_traits.h"

#include "Voicemeeter.UI/Graphics/ICanvas.h"
#include "Voicemeeter.UI/Graphics/IGlyph.h"

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

					virtual const ::std::valarray<double>& get_Position() const override;
					virtual const ::std::valarray<double>& get_Size() const override;

					virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override;
					virtual void Resize(const ::std::valarray<double>& vertex) override;

					inline IDWriteFactory7* get_pDwFactory() const {
						return m_pDwFactory.Get();
					};
					inline ID2D1Factory7* get_pD2dFactory() const {
						return m_pD2dFactory.Get();
					}
					inline ID2D1DeviceContext5* get_pD2dDeviceContext() const {
						return m_pD2dDeviceContext.Get();
					};
					inline IDXGISwapChain1* get_pDxgiSwapChain() const {
						return m_pDxgiSwapChain.Get();
					}
					inline const Palette& get_Palette() const {
						return *m_pPalette;
					}

				private:
					const ::std::valarray<double> m_point;
					::std::valarray<double> m_vertex;
					::Microsoft::WRL::ComPtr<IDWriteFactory7> m_pDwFactory;
					::Microsoft::WRL::ComPtr<ID2D1Factory7> m_pD2dFactory;
					::Microsoft::WRL::ComPtr<ID2D1DeviceContext5> m_pD2dDeviceContext;
					::Microsoft::WRL::ComPtr<IDXGISwapChain1> m_pDxgiSwapChain;
					::Microsoft::WRL::ComPtr<IDCompositionTarget> m_pCompositionTarget;
					::std::unique_ptr<Palette> m_pPalette;

					void ResetTarget();
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
					template<typename TBrush, typename Func,
						::std::enable_if_t<
							::estd::is_invocable_r<void, Func, ID2D1Brush**>::value,
							bool> = true>
					ID2D1Brush* get_pBrush(const Func& factory) const {
						const ::std::type_info& type{ typeid(TBrush) };
						::Microsoft::WRL::ComPtr<ID2D1Brush>& pBrush{
							m_cpBrush[
								reinterpret_cast<void*>(&
									const_cast<::std::type_info&>(type))]
						};
						if (!pBrush) {
							factory(&pBrush);
						}
						return pBrush.Get();
					};
					template<typename TGeometry, typename Func,
						::std::enable_if_t<
							::estd::is_invocable_r<void, Func, ID2D1GeometryRealization**, FLOAT>::value,
							bool> = true>
					ID2D1GeometryRealization* get_pGeometry(const Func& factory) const {
						const ::std::type_info& type{ typeid(TGeometry) };
						::Microsoft::WRL::ComPtr<ID2D1GeometryRealization>& pGeometry{
							m_cpGeometry[
								reinterpret_cast<void*>(&
									const_cast<::std::type_info&>(type))]
						};
						if (!pGeometry) {
							factory(&pGeometry, m_flatteringTolerance);
						}
						return pGeometry.Get();
					};

				private:
					const Theme m_theme;
					const Canvas& m_canvas;
					const FLOAT m_flatteringTolerance;
					mutable ::std::unordered_map<::std::wstring, ::Microsoft::WRL::ComPtr<IDWriteTextFormat>> m_cpTextFormat;
					mutable ::std::unordered_map<::std::wstring, ::Microsoft::WRL::ComPtr<IDWriteTextLayout>> m_cpTextLayout;
					mutable ::std::unordered_map<void*, ::Microsoft::WRL::ComPtr<ID2D1Brush>> m_cpBrush;
					mutable ::std::unordered_map<void*, ::Microsoft::WRL::ComPtr<ID2D1GeometryRealization>> m_cpGeometry;
				};
			}
		}
	}
}