#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "Canvas.h"

using namespace ::Voicemeeter::DeskBand::Windows;
using namespace ::Voicemeeter::DeskBand::UI::Graphics::D2D;

Canvas::Canvas(
	HWND hWnd
) : m_pDwFactory{ nullptr }
  , m_pD2dFactory{ nullptr }
  , m_pD2dRenderTarget{ nullptr } {
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
			::D2D1::SizeU(50, 50)
		),
		&m_pD2dRenderTarget
	), "Render target creation failed");
}

void Canvas::Redraw(linear_algebra::vector point, linear_algebra::vector vertex) {

}
void Canvas::Resize(linear_algebra::vector vertex) {
	ThrowIfFailed(m_pD2dRenderTarget->Resize(
		::D2D1::SizeU(vertex.x, vertex.y)
	), "Render target resize failed");
}