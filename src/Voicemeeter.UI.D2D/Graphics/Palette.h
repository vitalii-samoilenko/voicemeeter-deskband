#pragma once

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

#include "Theme.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				class Palette final {
				public:
					Palette(
						HWND hWnd,
						const Theme& theme
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
					};
					IDWriteTextFormat* get_pTextFormat(const ::std::wstring& fontFamily) const;
					IDWriteTextLayout* get_pTextLayout(const ::std::wstring& text, const ::std::wstring& fontFamily) const;
					template<typename TBrush, typename Func,
						::std::enable_if_t<
						::estd::is_invocable_r<void, Func, ID2D1Brush**>::value,
						bool> = true>
					ID2D1Brush* get_pBrush(const Func& factory) const {
						::Microsoft::WRL::ComPtr<ID2D1Brush>& pBrush{
							m_cpBrush[&typeid(TBrush)]
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
						::Microsoft::WRL::ComPtr<ID2D1GeometryRealization>& pGeometry{
							m_cpGeometry[&typeid(TGeometry)]
						};
						if (!pGeometry) {
							factory(&pGeometry, m_flatteringTolerance);
						}
						return pGeometry.Get();
					};

				private:
					const Theme m_theme;
					const FLOAT m_flatteringTolerance;
					::Microsoft::WRL::ComPtr<IDWriteFactory7> m_pDwFactory;
					::Microsoft::WRL::ComPtr<ID2D1Factory7> m_pD2dFactory;
					::Microsoft::WRL::ComPtr<ID2D1DeviceContext5> m_pD2dDeviceContext;
					::Microsoft::WRL::ComPtr<IDXGISwapChain1> m_pDxgiSwapChain;
					::Microsoft::WRL::ComPtr<IDCompositionTarget> m_pCompositionTarget;
					mutable ::std::unordered_map<::std::wstring, ::Microsoft::WRL::ComPtr<IDWriteTextFormat>> m_cpTextFormat;
					mutable ::std::unordered_map<::std::wstring, ::Microsoft::WRL::ComPtr<IDWriteTextLayout>> m_cpTextLayout;
					mutable ::std::unordered_map<const type_info*, ::Microsoft::WRL::ComPtr<ID2D1Brush>> m_cpBrush;
					mutable ::std::unordered_map<const type_info*, ::Microsoft::WRL::ComPtr<ID2D1GeometryRealization>> m_cpGeometry;
				};
			}
		}
	}
}