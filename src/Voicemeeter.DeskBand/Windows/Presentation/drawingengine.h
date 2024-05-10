#pragma once

#include <unordered_map>
#include <vector>
#include <utility>

#include <d3d11_4.h>
#pragma comment(lib, "d3d11")
#include <dwrite_3.h>
#pragma comment(lib, "dwrite")
#include <atlbase.h>

#include "style.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				class DrawingEngine {
				public:
					class Context {
					public:
						class Brush;
						class Text {
						public:
							Text(const Text&) = delete;
							Text(Text&&) = default;

							~Text() = default;

							Text& operator=(const Text&) = delete;
							Text& operator=(Text&&) = default;

							void Draw(const Brush& brush) const;

						private:
							friend class DrawingEngine;
							friend class Font;

							inline Text(Context& ctx)
								: m_ctx{ ctx } {

							};

							Context& m_ctx;
							CComPtr<IDWriteTextLayout4> m_pLayoutDw;
						};
						class Glyph {
						public:
							Glyph(const Glyph&) = delete;
							Glyph(Glyph&&) = default;

							~Glyph() = default;

							Glyph& operator=(const Glyph&) = delete;
							Glyph& operator=(Glyph&&) = default;

							void Draw(const Brush& brush) const;

						private:
							friend class DrawingEngine;
							friend class Outline;

							inline Glyph(Context& ctx)
								: m_ctx{ ctx } {

							};

							Context& m_ctx;
							CComPtr<ID2D1GeometryRealization> m_pRealizationD2;
						};
						class Brush {
						public:
							Brush(const Brush&) = delete;
							Brush(Brush&&) = default;

							~Brush() = default;

							Brush& operator=(const Brush&) = delete;
							Brush& operator=(Brush&&) = default;

						private:
							friend class DrawingEngine;
							friend class Color;
							friend class Text;
							friend class Glyph;

							inline Brush(Context& ctx)
								: m_ctx{ ctx }
								, m_pBrushD2{ NULL } {

							}

							Context& m_ctx;
							CComPtr<ID2D1SolidColorBrush> m_pBrushD2;
						};

						Context(const Context&) = delete;
						Context(Context&&) = default;

						~Context() = default;

						Context& operator=(const Context&) = delete;
						Context& operator=(Context&&) = default;

						void BeginDraw() const;
						void EndDraw() const;
						void Resize(UINT w, UINT h) const;

					private:
						friend class DrawingEngine;
						friend class Font;
						friend class Color;
						friend class Icon;

						IDWriteFactory7& m_fctDw;
						CComPtr<ID2D1DeviceContext6> m_pCtxDevD2;
						CComPtr<IDXGISwapChain4> m_pSwChDx;
						CComPtr<ID2D1Bitmap1> m_pBmpD2;

						inline Context(IDWriteFactory7& fctDw)
							: m_fctDw{ fctDw }
							, m_pCtxDevD2{ NULL }
							, m_pSwChDx{ NULL }
							, m_pBmpD2{ NULL } {

						};
					};

					class Manifest {
					public:
						class Font {
						public:
							Font(const Font&) = delete;
							Font(Font&&) = delete;

							~Font() = default;

							Font& operator=(const Font&) = delete;
							Font& operator=(Font&&) = delete;

							Context::Text Bind(Context& ctx,
								const LPCWSTR text, FLOAT w, FLOAT h
							) const;

						private:
							friend class DrawingEngine;
							friend class Manifest;

							CComPtr<IDWriteTextFormat3> m_pFmtDw;

							inline Font()
								: m_pFmtDw{ NULL } {

							}
						};
						class Color {
						public:
							Color(const Color&) = delete;
							Color(Color&&) = delete;

							~Color() = default;

							Color& operator=(const Color&) = delete;
							Color& operator=(Color&&) = delete;

							Context::Brush Bind(Context& ctx) const;

						private:
							friend class DrawingEngine;
							friend class Manifest;

							D2D1_COLOR_F m_colorD2;

							inline Color()
								: m_colorD2{} {

							};
						};
						class Outline {
						public:
							Outline(const Outline&) = delete;
							Outline(Outline&&) = delete;

							~Outline() = default;

							Outline& operator=(const Outline&) = delete;
							Outline& operator=(Outline&&) = delete;

							Context::Glyph Bind(Context& ctx) const;

						private:
							friend class DrawingEngine;
							friend class Manifest;

							FLOAT m_w;
							CComPtr<ID2D1Geometry> m_pGmtD2;

							inline Outline()
								: m_w{ 0.F }
								, m_pGmtD2{ NULL } {

							}
						};
						class Interior {
						public:
							Interior(const Interior&) = delete;
							Interior(Interior&&) = delete;

							~Interior() = default;

							Interior& operator=(const Interior&) = delete;
							Interior& operator=(Interior&&) = delete;

							Context::Glyph Bind(Context& ctx) const;

						private:
							friend class DrawingEngine;
							friend class Manifest;

							CComPtr<ID2D1Geometry> m_pGmtD2;

							inline Interior()
								: m_pGmtD2{ NULL } {

							}
						};

						inline const Font& get_fMain() const noexcept {
							return m_fMain;
						}
						inline const Color& get_cFront() const noexcept {
							return m_cFront;
						}
						inline const Color& get_cActive() const noexcept {
							return m_cActive;
						}
						inline const Outline& get_oBtnRound() const noexcept {
							return m_oBtnRound;
						}
						inline const Interior& get_iBtnRound() const noexcept {
							return m_iBtnRound;
						}

						Manifest(const Manifest&) = delete;
						Manifest(Manifest&&) = delete;

						~Manifest() = default;

						Manifest& operator=(const Manifest&) = delete;
						Manifest& operator=(Manifest&&) = delete;

					private:
						friend class DrawingEngine;

						Font m_fMain;
						Color m_cFront;
						Color m_cActive;
						Outline m_oBtnRound;
						Interior m_iBtnRound;

						inline Manifest()
							: m_fMain{}
							, m_cFront{}
							, m_cActive{}
							, m_oBtnRound{}
							, m_iBtnRound{} {

						}
					};

					explicit DrawingEngine(const Style& style);
					DrawingEngine() = delete;
					DrawingEngine(const DrawingEngine&) = delete;
					DrawingEngine(DrawingEngine&&) = delete;

					~DrawingEngine() = default;

					DrawingEngine& operator=(const DrawingEngine&) = delete;
					DrawingEngine& operator=(DrawingEngine&&) = delete;

					Context Initialize(
						HWND hWnd
					) const;

					private:
						CComPtr<ID3D11Device5> m_pDevD3;
						CComPtr<IDXGIDevice4> m_pDevDx;
						CComPtr<IDXGIAdapter3> m_pAdDx;
						CComPtr<IDXGIFactory5> m_pFctDx;
						CComPtr<ID2D1Factory7> m_pFctD2;
						CComPtr<ID2D1Device6> m_pDevD2;
						CComPtr<IDWriteFactory7> m_pFctDw;
						Manifest m_manifest;
				};
			}
		}
	}
}