#include <cmath>
#include <tuple>
#include <utility>

#include <windows.h>
#include <uxtheme.h>

#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "Canvas.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "uxtheme")

using namespace ::Voicemeeter::DeskBand::Windows;
using namespace ::Voicemeeter::DeskBand::UI::D2D::Graphics;

Canvas::Canvas(
	HWND hWnd
) : m_hWnd{ hWnd }
  , m_position{ ::linear_algebra::vectord::origin() }
  , m_vertex{ 50, 50 }
  , m_pDwFactory{ nullptr }
  , m_pD2dFactory{ nullptr }
  , m_pD2dRenderTarget{ nullptr }
  , m_pD2dGdiRenderTarget{ nullptr }
  , m_cpBrush{}
  , m_cpGeometry{}
  , m_pTextFormat{ nullptr } {
	ThrowIfFailed(CoInitialize(
		NULL
	), "COM initialization failed");

	ThrowIfFailed(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_pDwFactory),
		reinterpret_cast<IUnknown**>(m_pDwFactory.ReleaseAndGetAddressOf())
	), "DirectWrite factory creation failed");

	ThrowIfFailed(D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		IID_PPV_ARGS(&m_pD2dFactory)
	), "Direct2D factory creation failed");

	ThrowIfFailed(m_pD2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			0.F, 0.F,
			D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE),
		::D2D1::HwndRenderTargetProperties(
			hWnd,
			::D2D1::SizeU(m_vertex.x, m_vertex.y),
			D2D1_PRESENT_OPTIONS_RETAIN_CONTENTS),
		&m_pD2dRenderTarget
	), "Render target creation failed");

	ThrowIfFailed(m_pD2dRenderTarget->QueryInterface(
		IID_PPV_ARGS(&m_pD2dGdiRenderTarget)
	), "GDI render target creation failed");

	ThrowIfFailed(m_pDwFactory->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16,
		L"", //locale
		&m_pTextFormat
	), "Text format creation failed");
}

const ::linear_algebra::vectord& Canvas::get_Position() const {
	return m_position;
}
const ::linear_algebra::vectord& Canvas::get_Size() const {
	return m_vertex;
}

void Canvas::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	HDC hDc{};
	ThrowIfFailed(m_pD2dGdiRenderTarget->GetDC(
		D2D1_DC_INITIALIZE_MODE_COPY, &hDc
	), "Cannot get device context");
	RECT rc{
		static_cast<LONG>(::std::ceil(point.x)),
		static_cast<LONG>(::std::ceil(point.y)),
		static_cast<LONG>(::std::ceil(point.x + vertex.x)),
		static_cast<LONG>(::std::ceil(point.y + vertex.y))
	};
	//FillRect(hDc, &rc, CreateSolidBrush(RGB(255, 0, 0)));
	wDrawThemeParentBackground(m_hWnd, hDc, &rc);
	m_pD2dGdiRenderTarget->ReleaseDC(&rc);
}
void Canvas::Resize(const ::linear_algebra::vectord& vertex) {
	ThrowIfFailed(m_pD2dRenderTarget->Resize(
		::D2D1::SizeU(vertex.x, vertex.y)
	), "Render target resize failed");

	m_vertex = vertex;
}

bool Canvas::get_pBrush(const ::std::string& key, ID2D1SolidColorBrush** ppBrush) const {
	auto key_pBrush = m_cpBrush.find(key);
	if (key_pBrush != m_cpBrush.end()) {
		*ppBrush = key_pBrush->second.Get();

		return true;
	}

	::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush{ nullptr };
	ThrowIfFailed(m_pD2dRenderTarget->CreateSolidColorBrush(
		::D2D1::ColorF(0.F, 0.F, 0.F, 1.F),
		&pBrush
	), "Brush creation failed");
	::std::tie(key_pBrush, ::std::ignore) = m_cpBrush.insert({ key, ::std::move(pBrush) });

	*ppBrush = key_pBrush->second.Get();

	return false;
}

bool Canvas::get_pGeometry(const ::std::string& key, ID2D1PathGeometry** ppGeometry) const {
	auto key_pGeometry = m_cpGeometry.find(key);
	if (key_pGeometry != m_cpGeometry.end()) {
		*ppGeometry = key_pGeometry->second.Get();

		return true;
	}

	::Microsoft::WRL::ComPtr<ID2D1PathGeometry> pGeometry{ nullptr };
	ThrowIfFailed(m_pD2dFactory->CreatePathGeometry(
		&pGeometry
	), "Geometry creation failed");
	::std::tie(key_pGeometry, ::std::ignore) = m_cpGeometry.insert({ key, ::std::move(pGeometry) });

	*ppGeometry = key_pGeometry->second.Get();

	return false;
}