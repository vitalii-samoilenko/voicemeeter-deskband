#define _USE_MATH_DEFINES

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
  , m_angle{ 0 }
  , m_maskPoint{ 0., 0. }
  , m_labelPoint{ 0., 0. }
  , m_labelVertex{ 0., 0. }
  , m_labelMaskPoint{ 0., 0. }
  , m_indicatorPoint{ 0., 0. }
  , m_indicatorVertex{ 0., 0. }
  , m_indicatorMaskPoint{ 0., 0. } {

}

void Knob::Execute() {
	if (m_changed.test(size)) {
		m_changed.reset(size);
		m_maskPoint = get_Palette()
			.get_Atlas()
				.MapPosition(
					Atlas::Specification::Knob::Frame::Point::X,
					Atlas::Specification::Knob::Frame::Point::Y);
		m_labelVertex = get_Palette()
			.get_Atlas()
				.MapSize(Atlas::Specification::Block::Width, Atlas::Specification::Block::Height);
		m_changed.set(position);
		m_changed.set(label);
		m_indicatorVertex = get_Palette()
			.get_Atlas()
				.MapSize(
					2.F * Atlas::Specification::Knob::Indicator::Radius,
					2.F * Atlas::Specification::Knob::Indicator::Radius);
		m_indicatorMaskPoint = get_Palette()
			.get_Atlas()
				.MapPosition(
					Atlas::Specification::Knob::Indicator::Point::X,
					Atlas::Specification::Knob::Indicator::Point::Y);
	}
	if (m_changed.test(position)) {
		m_changed.reset(position);
		m_labelPoint = get_Position()
			+ (get_Size() - m_labelVertex) / 2.;
		m_changed.set(angle);
	}
	if (m_changed.test(label)) {
		m_changed.reset(label);
		m_labelMaskPoint = get_Palette()
			.get_Atlas()
				.MapPosition(
					Atlas::Specification::Knob::Label::Strip::Point::X + m_label % Atlas::Specification::Width,
					Atlas::Specification::Knob::Label::Strip::Point::Y + m_label / Atlas::Specification::Width);
	}
	if (m_changed.test(angle)) {
		m_changed.reset(angle);
		constexpr double R{
			Atlas::Specification::Knob::Frame::Radius
			- Atlas::Specification::Knob::Frame::Stroke
			- 2. * Atlas::Specification::Knob::Indicator::Radius
		};
		double phi{ m_angle / 18000. * M_PI };
		m_indicatorPoint = get_Position()
			+ get_Size() / get_BaseSize()
				* (::std::valarray<double>{ R * ::std::cos(phi), R * ::std::sin(phi) }
					+ Atlas::Specification::Knob::Frame::Radius - Atlas::Specification::Knob::Indicator::Radius);
	}

	get_Palette()
		.get_pDeviceContext()
			->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
	FillOpacityMask(
		m_frameColor,
		get_Position(),
		get_Size(),
		m_maskPoint);
	get_Palette()
		.get_pDeviceContext()
			->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);

	FillOpacityMask(
		m_labelColor,
		m_labelPoint,
		m_labelVertex,
		m_labelMaskPoint);

	FillOpacityMask(
		get_Palette()
			.get_Theme()
				.Indicator,
		m_indicatorPoint,
		m_indicatorVertex,
		m_indicatorMaskPoint);
};