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

#include "Environment/ITimer.h"
#include "Voicemeeter.UI/Graphics/ICanvas.h"
#include "Voicemeeter.UI/Graphics/IGlyph.h"

#include "Theme.h"

using namespace ::Voicemeeter::UI::Graphics;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				class Palette;
				class Queue;

				class Canvas final : public ICanvas {
				public:
					Canvas(
						HWND hWnd,
						const Theme& theme,
						::Environment::ITimer& timer
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
					inline ID2D1DeviceContext5* get_pD2dDeviceContext() const {
						return m_pD2dDeviceContext.Get();
					};
					inline IDXGISwapChain1* get_pDxgiSwapChain() const {
						return m_pDxgiSwapChain.Get();
					}
					inline const Palette& get_Palette() const {
						return *m_pPalette;
					}
					inline Queue& get_Queue() const {
						return *m_pQueue;
					}

				private:
					::linear_algebra::vectord m_point;
					::linear_algebra::vectord m_vertex;
					::Microsoft::WRL::ComPtr<IDWriteFactory7> m_pDwFactory;
					::Microsoft::WRL::ComPtr<ID2D1Factory7> m_pD2dFactory;
					::Microsoft::WRL::ComPtr<ID2D1DeviceContext5> m_pD2dDeviceContext;
					::Microsoft::WRL::ComPtr<IDXGISwapChain1> m_pDxgiSwapChain;
					::Microsoft::WRL::ComPtr<IDCompositionTarget> m_pCompositionTarget;
					::std::unique_ptr<Palette> m_pPalette;
					::std::unique_ptr<Queue> m_pQueue;

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
					ID2D1SolidColorBrush* get_pBrush(const ::D2D1::ColorF& color) const;
					template<typename Func,
						::std::enable_if_t<
							::estd::is_invocable_r<void, Func, ID2D1GeometryRealization**, FLOAT>::value,
							bool> = true>
					ID2D1GeometryRealization* get_pGeometry(const ::std::type_info& type, const Func& factory) const {
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
					Theme m_theme;
					const Canvas& m_canvas;
					const FLOAT m_flatteringTolerance;
					mutable ::std::unordered_map<void*, ::Microsoft::WRL::ComPtr<IDWriteTextFormat>> m_cpTextFormat;
					mutable ::std::unordered_map<void*, ::Microsoft::WRL::ComPtr<IDWriteTextLayout>> m_cpTextLayout;
					mutable ::std::unordered_map<void*, ::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> m_cpBrush;
					mutable ::std::unordered_map<void*, ::Microsoft::WRL::ComPtr<ID2D1GeometryRealization>> m_cpGeometry;
				};

				class Queue final {
				public:
					Queue(
						::Environment::ITimer& timer,
						Canvas& canvas
					);
					Queue() = delete;
					Queue(const Queue&) = delete;
					Queue(Queue&&) = delete;

					~Queue() = default;

					Queue& operator=(const Queue&) = delete;
					Queue& operator=(Queue&&) = delete;

					inline void Push(IGlyph& glyph) {
						m_cpGlyph.insert(&glyph);
					};

				private:
					Canvas& m_canvas;
					::std::unordered_set<IGlyph*> m_cpGlyph;
				};
			}
		}
	}
}