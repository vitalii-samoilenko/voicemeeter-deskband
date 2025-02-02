#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include <cmath>

#include "Canvas.h"

using namespace ::Voicemeeter::UI::D2D::Graphics;

void Canvas::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	using RGBA = ::Voicemeeter::UI::Cherry::Graphics::Theme::RGBA;

	const ::std::valarray<double>& color{
		m_palette.get_Theme()
			.Background
	};
}
void Canvas::Resize(const ::std::valarray<double>& vertex) {
	m_vertex[0] = ::std::max(8., vertex[0]);
	m_vertex[1] = ::std::max(8., vertex[1]);

	m_palette.get_Instrumentation()
		.get_pDeviceContext()
			->SetTarget(nullptr);

	::Windows::ThrowIfFailed(m_palette.get_Instrumentation()
		.get_pSwapChain()
			->ResizeBuffers(
				0U,
				static_cast<UINT>(::std::ceil(m_vertex[0])),
				static_cast<UINT>(::std::ceil(m_vertex[1])),
				DXGI_FORMAT_UNKNOWN,
				0U
	), "Swap chain resize failed");

	::Microsoft::WRL::ComPtr<IDXGISurface> pBackBuffer{ nullptr };
	::Windows::ThrowIfFailed(m_palette.get_Instrumentation()
		.get_pSwapChain()
			->GetBuffer(
				0U,
				IID_PPV_ARGS(&pBackBuffer)
	), "Could not get back buffer");

	D2D1_BITMAP_PROPERTIES1 bitmapProperties{
		D2D1_PIXEL_FORMAT{
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED
		},
		0.F, 0.F,
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		nullptr
	};
	::Microsoft::WRL::ComPtr<ID2D1Bitmap1> pBitmap{ nullptr };
	::Windows::ThrowIfFailed(m_palette.get_Instrumentation()
		.get_pDeviceContext()
			->CreateBitmapFromDxgiSurface(
				pBackBuffer.Get(),
				&bitmapProperties,
				&pBitmap
	), "Bitmap creation failed");
	m_palette.get_Instrumentation()
		.get_pDeviceContext()
			->SetTarget(pBitmap.Get());
}