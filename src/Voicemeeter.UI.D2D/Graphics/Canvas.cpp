#include <cmath>

#include "Canvas.h"

using namespace ::Voicemeeter::UI::D2D::Graphics;

Canvas::Canvas(
	const Palette& palette
) : m_point{ 0., 0. }
  , m_vertex{ 0., 0. }
  , m_palette{ palette } {

}

const ::std::valarray<double>& Canvas::get_Position() const {
	return m_point;
}
const ::std::valarray<double>& Canvas::get_Size() const {
	return m_vertex;
}

void Canvas::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	m_palette.get_pDeviceContext()
		->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
	m_palette.get_pBrush()
		->SetColor(m_palette.get_Theme()
			.Background);
	m_palette.get_pDeviceContext()
		->FillRectangle(
			::D2D1::RectF(
				static_cast<FLOAT>(point[0]),
				static_cast<FLOAT>(point[1]),
				static_cast<FLOAT>(point[0] + vertex[0]),
				static_cast<FLOAT>(point[1] + vertex[1])),
			m_palette.get_pBrush());
	m_palette.get_pDeviceContext()
		->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
}
void Canvas::Resize(const ::std::valarray<double>& vertex) {
	m_vertex[0] = ::std::max(8., vertex[0]);
	m_vertex[1] = ::std::max(8., vertex[1]);

	m_palette.get_pDeviceContext()
		->SetTarget(nullptr);

	::Windows::ThrowIfFailed(m_palette.get_pSwapChain()
		->ResizeBuffers(
			0U,
			static_cast<UINT>(::std::ceil(m_vertex[0])),
			static_cast<UINT>(::std::ceil(m_vertex[1])),
			DXGI_FORMAT_UNKNOWN,
			0U
	), "Swap chain resize failed");

	::Microsoft::WRL::ComPtr<IDXGISurface> pBackBuffer{ nullptr };
	::Windows::ThrowIfFailed(m_palette.get_pSwapChain()
		->GetBuffer(
			0U, IID_PPV_ARGS(&pBackBuffer)
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
	::Windows::ThrowIfFailed(m_palette.get_pDeviceContext()
		->CreateBitmapFromDxgiSurface(
			pBackBuffer.Get(),
			&bitmapProperties,
			&pBitmap
	), "Bitmap creation failed");
	m_palette.get_pDeviceContext()
		->SetTarget(pBitmap.Get());
}