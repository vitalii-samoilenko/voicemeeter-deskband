#include "Windows/Wrappers.h"

#include "Palette.h"

using namespace ::Voicemeeter::UI::D2D::Graphics;

Bundle::Bundle(
	Palette& palette,
	const ::std::valarray<double>& baseVertex
) : m_palette{ palette }
  , m_point{ 0., 0. }
  , m_vertex{ baseVertex }
  , m_baseVertex{ baseVertex } {

}

void Bundle::Execute() {
	m_palette.get_pD2dDeviceContext()
		->SetTransform(
			::D2D1::Matrix3x2F::Scale(
				static_cast<FLOAT>(m_vertex[0] / m_baseVertex[0]),
				static_cast<FLOAT>(m_vertex[1] / m_baseVertex[1]))
			* ::D2D1::Matrix3x2F::Translation(
				static_cast<FLOAT>(m_point[0]),
				static_cast<FLOAT>(m_point[1])));
	m_palette.get_pD2dDeviceContext()
		->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
	m_palette.get_pD2dDeviceContext()
		->FillRectangle(
			::D2D1::RectF(
				0.F,
				0.F,
				static_cast<FLOAT>(m_baseVertex[0]),
				static_cast<FLOAT>(m_baseVertex[1])
			),
			m_palette.get_pBrush<Palette>(
				[this](ID2D1Brush** ppBrush)->void {
					ID2D1SolidColorBrush* pBrush{ nullptr };
					::Windows::ThrowIfFailed(m_palette.get_pD2dDeviceContext()
						->CreateSolidColorBrush(
							m_palette.get_Theme()
							.Background,
							&pBrush
						), "Brush creation failed");
					*ppBrush = pBrush;
				}));
	m_palette.get_pD2dDeviceContext()
		->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
}