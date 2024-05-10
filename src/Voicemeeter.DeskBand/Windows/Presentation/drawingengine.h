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
						class Text {
						public:
							Text(const Text&) = delete;
							Text(Text&&) = default;

							~Text() = default;

							Text& operator=(const Text&) = delete;
							Text& operator=(Text&&) = default;

						private:
							friend class DrawingEngine;
							friend class Font;

							Text() = default;

							CComPtr<IDWriteTextFormat3> m_pFmtDw;
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

							Brush() = default;

							CComPtr<ID2D1SolidColorBrush> m_pBrushD2;
						};
						class Icon {
						public:
							Icon(const Icon&) = delete;
							Icon(Icon&&) = default;

							~Icon() = default;

							Icon& operator=(const Icon&) = delete;
							Icon& operator=(Icon&&) = default;

						private:
							friend class DrawingEngine;
							friend class Glyph;

							Icon() = default;

							//CComPtr<ID2D1SolidColorBrush> m_pBrushD2;
						};

						Context(const Context&) = delete;
						Context(Context&&) = default;

						~Context() = default;

						Context& operator=(const Context&) = delete;
						Context& operator=(Context&&) = default;

						void BeginDraw();
						void EndDraw();
						void Resize(UINT w, UINT h);

					private:
						friend class DrawingEngine;
						friend class Font;
						friend class Color;
						friend class Icon;

						CComPtr<ID2D1DeviceContext6> m_pCtxDevD2;
						CComPtr<IDXGISwapChain4> m_pSwChDx;
						CComPtr<ID2D1Bitmap1> m_pBmpD2;

						Context() = default;
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

							Context::Text Bind(const Context& ctx);

						private:
							friend class DrawingEngine;
							friend class Manifest;

							CComPtr<IDWriteTextFormat3> m_pFmtDw;

							Font() = default;
						};
						class Color {
						public:
							Color(const Color&) = delete;
							Color(Color&&) = delete;

							~Color() = default;

							Color& operator=(const Color&) = delete;
							Color& operator=(Color&&) = delete;

							Context::Brush Bind(const Context& ctx);

						private:
							friend class DrawingEngine;
							friend class Manifest;

							D2D1_COLOR_F m_color;

							Color() = default;
						};
						class Glyph {
						public:
							Glyph(const Glyph&) = delete;
							Glyph(Glyph&&) = delete;

							~Glyph() = default;

							Glyph& operator=(const Glyph&) = delete;
							Glyph& operator=(Glyph&&) = delete;

							Context::Icon Bind(const Context& ctx);

						private:
							friend class DrawingEngine;
							friend class Manifest;

							CComPtr<ID2D1Geometry> m_pGmtD2;

							Glyph() = default;
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
						inline const Glyph& get_iBtnRoundFrame() const noexcept {
							return m_iBtnRoundFrame;
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
						Glyph m_iBtnRoundFrame;

						Manifest() = default;
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
					);

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