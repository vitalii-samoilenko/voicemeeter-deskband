#include "Vban.h"

using namespace Voicemeeter::UI::D2D::Graphics::Bundles;

Vban::Vban(
	Graphics::Palette& palette
) : Bundle{ palette, { Atlas::Specification::Vban::Frame::Width, Atlas::Specification::Vban::Frame::Height } }
  , m_color{
		get_Palette()
			.get_Theme()
				.Inactive
	} {

}

void Vban::Execute() {
	::std::valarray<double> vertex{ get_Size() };
	::std::valarray<double> point{ get_Position() };
	::std::valarray<double> maskPoint{
		get_Palette()
			.get_Atlas()
				.MapPosition(
					Atlas::Specification::Vban::Frame::Point::X,
					Atlas::Specification::Vban::Frame::Point::Y)
	};
	get_Palette()
		.get_pDeviceContext()
			->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
	FillOpacityMask(
		m_color,
		point,
		vertex,
		maskPoint);
	get_Palette()
		.get_pDeviceContext()
			->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
};