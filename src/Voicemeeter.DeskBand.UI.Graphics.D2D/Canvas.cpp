#include <tuple>
#include <utility>

#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "Canvas.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

using namespace ::Voicemeeter::DeskBand::Windows;
using namespace ::Voicemeeter::DeskBand::UI::Graphics::D2D;

Canvas::Canvas(
	HWND hWnd
) : m_position{ ::linear_algebra::vectord::origin() }
  , m_vertex{ 50, 50 }
  , m_pDwFactory{ nullptr }
  , m_pD2dFactory{ nullptr }
  , m_pD2dRenderTarget{ nullptr }
  , m_pBackgroundBrush{ nullptr }
  , m_cpBrush{} {
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
		::D2D1::RenderTargetProperties(),
		::D2D1::HwndRenderTargetProperties(
			hWnd,
			::D2D1::SizeU(m_vertex.x, m_vertex.y)
		),
		&m_pD2dRenderTarget
	), "Render target creation failed");
}

const ::linear_algebra::vectord& Canvas::get_Position() const {
	return m_position;
}
const ::linear_algebra::vectord& Canvas::get_Size() const {
	return m_vertex;
}

void Canvas::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	m_pD2dRenderTarget->BeginDraw();
	m_pD2dRenderTarget->Clear(m_pBackgroundBrush->GetColor());
	ThrowIfFailed(m_pD2dRenderTarget->EndDraw(
	), "Render failed");
}
void Canvas::Resize(const ::linear_algebra::vectord& vertex) {
	ThrowIfFailed(m_pD2dRenderTarget->Resize(
		::D2D1::SizeU(vertex.x, vertex.y)
	), "Render target resize failed");

	m_vertex = vertex;
}

ID2D1SolidColorBrush* Canvas::get_pBrush(DWORD color) const {
	auto color_pBrush = m_cpBrush.find(color);
	if (color_pBrush == m_cpBrush.end()) {
		::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush{ nullptr };
		ThrowIfFailed(m_pD2dRenderTarget->CreateSolidColorBrush(
			::D2D1::ColorF(color),
			&pBrush
		), "Brush creation failed");
		::std::tie(color_pBrush, ::std::ignore) = m_cpBrush.insert({ color, ::std::move(pBrush) });
	}
	return color_pBrush->second.Get();
}