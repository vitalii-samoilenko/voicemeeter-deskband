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
	FillOpacityMask(
		m_color,
		get_Position(),
		get_Size(),
		m_maskPoint);
	get_Palette()
		.get_pDeviceContext()
			->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
};