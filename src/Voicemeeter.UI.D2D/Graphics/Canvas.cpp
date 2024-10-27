#include <cmath>
#include <tuple>
#include <utility>

#include "Windows/Wrappers.h"

#include "Canvas.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "uxtheme")

using namespace ::Voicemeeter::UI::D2D::Graphics;

Canvas::Canvas(
	HWND hWnd,
	const Theme& theme
) : m_hWnd{ hWnd }
  , m_pPalette{ nullptr }
  , m_position{ ::linear_algebra::vectord::origin() }
  , m_vertex{ 50, 50 }
  , m_pDwFactory{ nullptr }
  , m_pD2dFactory{ nullptr }
  , m_pD2dRenderTarget{ nullptr }
  , m_pD2dGdiRenderTarget{ nullptr } {
	::Windows::ThrowIfFailed(CoInitialize(
		NULL
	), "COM initialization failed");

	::Windows::ThrowIfFailed(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_pDwFactory),
		reinterpret_cast<IUnknown**>(m_pDwFactory.ReleaseAndGetAddressOf())
	), "DirectWrite factory creation failed");

	::Windows::ThrowIfFailed(D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
#ifndef NDEBUG
		D2D1_FACTORY_OPTIONS{
			D2D1_DEBUG_LEVEL_INFORMATION
		},
#endif // !NDEBUG
		m_pD2dFactory.ReleaseAndGetAddressOf()
	), "Direct2D factory creation failed");

	::Windows::ThrowIfFailed(m_pD2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			0.F, 0.F,
			D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE),
		::D2D1::HwndRenderTargetProperties(
			hWnd,
			::D2D1::SizeU(
				static_cast<UINT32>(::std::ceil(m_vertex.x)),
				static_cast<UINT32>(::std::ceil(m_vertex.y))),
			D2D1_PRESENT_OPTIONS_RETAIN_CONTENTS),
		&m_pD2dRenderTarget
	), "Render target creation failed");

	::Windows::ThrowIfFailed(m_pD2dRenderTarget->QueryInterface(
		IID_PPV_ARGS(&m_pD2dGdiRenderTarget)
	), "GDI render target creation failed");

	m_pPalette.reset(new Palette{ theme, *this });
}

const ::linear_algebra::vectord& Canvas::get_Position() const {
	return m_position;
}
const ::linear_algebra::vectord& Canvas::get_Size() const {
	return m_vertex;
}

void Canvas::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	HDC hDc{};
	::Windows::ThrowIfFailed(m_pD2dGdiRenderTarget->GetDC(
		D2D1_DC_INITIALIZE_MODE_COPY, &hDc
	), "Cannot get device context");
	RECT rc{
		static_cast<LONG>(::std::floor(point.x)),
		static_cast<LONG>(::std::floor(point.y)),
		static_cast<LONG>(::std::ceil(point.x + vertex.x)),
		static_cast<LONG>(::std::ceil(point.y + vertex.y))
	};
#ifdef NDEBUG
	::Windows::wDrawThemeParentBackground(m_hWnd, hDc, &rc);
#else
	FillRect(hDc, &rc, CreateSolidBrush(RGB(44, 61, 77)));
#endif
	m_pD2dGdiRenderTarget->ReleaseDC(&rc);
}
void Canvas::Resize(const ::linear_algebra::vectord& vertex) {
	::Windows::ThrowIfFailed(m_pD2dRenderTarget->Resize(
		::D2D1::SizeU(
			static_cast<UINT32>(::std::ceil(vertex.x)),
			static_cast<UINT32>(::std::ceil(vertex.y)))
	), "Render target resize failed");

	m_vertex = vertex;
}