#pragma once

#include <unordered_map>
#include <vector>
#include <utility>

#include <d3d11_4.h>
#pragma comment(lib, "d3d11_4")
#include <dwrite_3.h>
#pragma comment(lib, "dwrite_3")
#include <atlbase.h>

#include "style.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				class DrawingEngine {
					const Style& m_rStyle;
					CComPtr<ID3D11Device5> m_pDevD3d11;
					CComPtr<IDXGIDevice4> m_pDevDxgi;
					CComPtr<IDXGIAdapter3> m_pAdDxgi;
					CComPtr<IDXGIFactory5> m_pFctDxgi;
					CComPtr<ID2D1Factory7> m_pFctD2d1;
					CComPtr<ID2D1Device6> m_pDevD2d1;
					CComPtr<IDWriteFactory7> m_pFctDw;

				public:
					class Manifest {
						friend class DrawingEngine;

					public:
						class Primitive {

						};
					};

					class Context {
						friend class DrawingEngine;

						CComPtr<ID2D1DeviceContext6> m_pCtxDevD2d1;
						CComPtr<IDXGISwapChain4> m_pSwChDxgi;
						CComPtr<ID2D1Bitmap1> m_pBmpD2d1;

						Context();

					public:
						class Resource {

						};

						Context(const Context&) = delete;
						Context(Context&&) = default;

						~Context() = default;

						Context& operator=(const Context&) = delete;
						Context& operator=(Context&&) = default;

						Resource Bind(const Manifest::Primitive& rPrimitive);
						void BeginDraw();
						void EndDraw();
						void Resize(UINT w, UINT h);
					};

					explicit DrawingEngine(
						const Style& rStyle
					);
					DrawingEngine() = delete;
					DrawingEngine(const DrawingEngine&) = delete;
					DrawingEngine(DrawingEngine&&) = default;

					~DrawingEngine() = default;

					DrawingEngine& operator=(const DrawingEngine&) = delete;
					DrawingEngine& operator=(DrawingEngine&&) = default;

					Context Initialize(
						HWND hWnd
					);
				};
			}
		}
	}
}