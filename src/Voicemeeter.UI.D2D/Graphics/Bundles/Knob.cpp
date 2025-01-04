#include <cmath>

#include "Knob.h"

using namespace ::Voicemeeter::UI::D2D::Graphics::Bundles;

Knob::Knob(
	Graphics::Palette& palette,
	size_t label
) : Bundle{ palette, { 2. * Atlas::Specification::Knob::Frame::Radius, 2. * Atlas::Specification::Knob::Frame::Radius } }
  , m_label{ label }
  , m_frameColor{
		get_Palette()
			.get_Theme()
				.Inactive
	}
  , m_labelColor{
		get_Palette()
			.get_Theme()
				.Inactive
	}
  , m_angle{ 0. } {

}

void Knob::Execute() {
	::std::valarray<double> vertex{ get_Size() };
	::std::valarray<double> point{ get_Position() };
	::std::valarray<double> maskPoint{
		get_Palette()
			.get_Atlas()
				.MapPosition(
					Atlas::Specification::Knob::Frame::Point::X,
					Atlas::Specification::Knob::Frame::Point::Y)
	};
	get_Palette()
		.get_pDeviceContext()
			->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
	FillOpacityMask(
		m_frameColor,
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
	maskPoint = get_Palette()
		.get_Atlas()
			.MapPosition(
				Atlas::Specification::Knob::Label::Strip::Point::X + m_label % Atlas::Specification::Width,
				Atlas::Specification::Knob::Label::Strip::Point::Y + m_label / Atlas::Specification::Width);
	FillOpacityMask(
		m_labelColor,
		point,
		vertex,
		maskPoint);

	vertex = get_Palette()
		.get_Atlas()
			.MapSize(
				2.F * Atlas::Specification::Knob::Indicator::Radius,
				2.F * Atlas::Specification::Knob::Indicator::Radius);
	constexpr double R{
		Atlas::Specification::Knob::Frame::Radius
		- Atlas::Specification::Knob::Frame::Stroke
		- 2. * Atlas::Specification::Knob::Indicator::Radius
	};
	point = get_Position() + get_Size() / get_BaseSize()
		* (::std::valarray<double>{ R * ::std::cos(m_angle), R * ::std::sin(m_angle) }
			+ Atlas::Specification::Knob::Frame::Radius - Atlas::Specification::Knob::Indicator::Radius);
	maskPoint = get_Palette()
		.get_Atlas()
			.MapPosition(
				Atlas::Specification::Knob::Indicator::Point::X,
				Atlas::Specification::Knob::Indicator::Point::Y);
	FillOpacityMask(
		get_Palette()
			.get_Theme()
				.Indicator,
		point,
		vertex,
		maskPoint);
};