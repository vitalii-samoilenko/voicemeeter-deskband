#include "Plug.h"

using namespace Voicemeeter::UI::D2D::Graphics::Bundles;

Plug::Plug(
	Graphics::Palette& palette,
	size_t label
) : Bundle{ palette, { Atlas::Specification::Plug::Frame::Width, Atlas::Specification::Plug::Frame::Height } }
  , m_label{ label }
  , m_color{
		get_Palette()
			.get_Theme()
				.Inactive
  } {

}

void Plug::Execute() {
	::std::valarray<double> vertex{ get_Size() };
	::std::valarray<double> point{ get_Position() };
	::std::valarray<double> maskPoint{
		get_Palette()
			.get_Atlas()
				.MapPosition(
					Atlas::Specification::Plug::Frame::Point::X,
					Atlas::Specification::Plug::Frame::Point::Y)
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

	vertex = get_Palette()
		.get_Atlas()
			.MapSize(Atlas::Specification::Block::Width, Atlas::Specification::Block::Height);
	point = get_Position() + (get_Size() - vertex) / 2.;
	point[0] += get_Palette()
		.get_Atlas()
			.MapSize(
				Atlas::Specification::Plug::Indicator::Tip::X - Atlas::Specification::Plug::Indicator::Start::X,
				0.F)[0] * 0.7;
	maskPoint = get_Palette()
		.get_Atlas()
			.MapPosition(
				Atlas::Specification::Plug::Label::Strip::Point::X + m_label % Atlas::Specification::Width,
				Atlas::Specification::Plug::Label::Strip::Point::Y + m_label / Atlas::Specification::Width);
	FillOpacityMask(
		m_color,
		point,
		vertex,
		maskPoint);
}