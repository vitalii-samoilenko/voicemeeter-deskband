#pragma once

#include <chrono>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <valarray>

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
				class Bundle;

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
							::estd::is_invocable_r<void, Func, ID2D1Geometry**>::value,
							bool> = true>
					ID2D1Geometry* get_pGeometry(const Func& factory) const {
						::Microsoft::WRL::ComPtr<ID2D1Geometry>& pGeometry{
							m_cpGeometry[&typeid(TGeometry)]
						};
						if (!pGeometry) {
							factory(&pGeometry);
						}
						return pGeometry.Get();
					};
					template<typename TGeometry, typename Func,
						::std::enable_if_t<
							::estd::is_invocable_r<void, Func, ID2D1GeometryRealization**>::value,
						bool> = true>
					ID2D1GeometryRealization* get_pGeometryRealization(const Func& factory) const {
						::Microsoft::WRL::ComPtr<ID2D1GeometryRealization>& pGeometryRealization{
							m_cpGeometryRealization[&typeid(TGeometry)]
						};
						if (!pGeometryRealization) {
							factory(&pGeometryRealization);
						}
						return pGeometryRealization.Get();
					};
					inline ::std::chrono::nanoseconds get_Elapsed() const {
						return ::std::chrono::duration_cast<::std::chrono::nanoseconds>(m_now - m_past);
					};

					inline void Tick() {
						m_past = m_now;
						m_now = ::std::chrono::high_resolution_clock::now();
					};
					inline void Queue(Bundle& bundle) {
						m_queue.insert(&bundle);
					};
					inline ::std::unordered_set<Bundle*> Dequeue() {
						return ::std::move(m_queue);
					};

					static inline void Blend(::D2D1::ColorF& dst, const ::D2D1::ColorF& src, FLOAT alpha) {
						dst.r = dst.r * (1.F - alpha) + src.r * alpha;
						dst.g = dst.g * (1.F - alpha) + src.g * alpha;
						dst.b = dst.b * (1.F - alpha) + src.b * alpha;
					};

				private:
					const Theme m_theme;
					::Microsoft::WRL::ComPtr<IDWriteFactory7> m_pDwFactory;
					::Microsoft::WRL::ComPtr<ID2D1Factory7> m_pD2dFactory;
					::Microsoft::WRL::ComPtr<ID2D1DeviceContext5> m_pD2dDeviceContext;
					::Microsoft::WRL::ComPtr<IDXGISwapChain1> m_pDxgiSwapChain;
					::Microsoft::WRL::ComPtr<IDCompositionTarget> m_pCompositionTarget;
					mutable ::std::unordered_map<::std::wstring, ::Microsoft::WRL::ComPtr<IDWriteTextFormat>> m_cpTextFormat;
					mutable ::std::unordered_map<::std::wstring, ::Microsoft::WRL::ComPtr<IDWriteTextLayout>> m_cpTextLayout;
					mutable ::std::unordered_map<const type_info*, ::Microsoft::WRL::ComPtr<ID2D1Brush>> m_cpBrush;
					mutable ::std::unordered_map<const type_info*, ::Microsoft::WRL::ComPtr<ID2D1Geometry>> m_cpGeometry;
					mutable ::std::unordered_map<const type_info*, ::Microsoft::WRL::ComPtr<ID2D1GeometryRealization>> m_cpGeometryRealization;
					::std::chrono::high_resolution_clock::time_point m_now;
					::std::chrono::high_resolution_clock::time_point m_past;
					::std::unordered_set<Bundle*> m_queue;
				};

				class Bundle {
				public:
					Bundle() = delete;
					Bundle(const Bundle&) = delete;
					Bundle(Bundle&&) = delete;

					~Bundle() = default;

					Bundle& operator=(const Bundle&) = delete;
					Bundle& operator=(Bundle&&) = delete;

					inline Palette& get_Palette() {
						return m_palette;
					};
					inline const ::std::valarray<double>& get_Position() const {
						return m_point;
					};
					inline void set_Position(const ::std::valarray<double>& value) {
						m_point = value;
					};
					inline const ::std::valarray<double>& get_Size() const {
						return m_vertex;
					};
					inline void set_Size(const ::std::valarray<double>& value) {
						m_vertex = value;
					};
					inline const ::std::valarray<double>& get_BaseSize() const {
						return m_baseVertex;
					};

					virtual void Execute();

				protected:
					Bundle(
						Palette& palette,
						const ::std::valarray<double>& baseVertex
					);

				private:
					Palette& m_palette;
					::std::valarray<double> m_point;
					::std::valarray<double> m_vertex;
					const ::std::valarray<double> m_baseVertex;
				};
			}
		}
	}
}