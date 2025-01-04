#include "Vban.h"

using namespace Voicemeeter::UI::D2D::Graphics::Bundles;

Vban::Vban(
	Graphics::Palette& palette
) : Bundle{ palette, { Atlas::Specification::Vban::Frame::Width, Atlas::Specification::Vban::Frame::Height } }
  , m_color{
		get_Palette()
			.get_Theme()
				.Inactive
	}
  , m_maskPoint{ 0., 0. } {

}

void Vban::Execute() {
	if (m_changed.test(size)) {
		m_changed.reset(size);
		m_maskPoint = get_Palette()
			.get_Atlas()
				.MapPosition(
					Atlas::Specification::Vban::Frame::Point::X,
					Atlas::Specification::Vban::Frame::Point::Y);
	}

	get_Palette()
		.get_pDeviceContext()
			->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
	get_Palette()
		.get_pBrush()
			->SetColor(m_color);
	get_Palette()
		.get_pDeviceContext()
			->FillOpacityMask(
				get_Palette()
					.get_Atlas()
						.get_pBitmap(),
				get_Palette()
					.get_pBrush(),
				Atlas::RectF(get_Position(), get_Size()),
				Atlas::RectF(m_maskPoint, get_Size()));
	get_Palette()
		.get_pDeviceContext()
			->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
};