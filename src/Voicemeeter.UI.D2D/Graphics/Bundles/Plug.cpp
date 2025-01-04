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
  }
  , m_maskPoint{ 0., 0. }
  , m_labelPoint{ 0., 0. }
  , m_labelVertex{ 0., 0. }
  , m_labelMaskPoint{ 0., 0. } {

}

void Plug::Execute() {
	if (m_changed.test(size)) {
		m_changed.reset(size);
		m_maskPoint = get_Palette()
			.get_Atlas()
				.MapPosition(
					Atlas::Specification::Plug::Frame::Point::X,
					Atlas::Specification::Plug::Frame::Point::Y);
		m_labelVertex = get_Palette()
			.get_Atlas()
				.MapSize(Atlas::Specification::Block::Width, Atlas::Specification::Block::Height);
		m_changed.set(position);
		m_changed.set(label);
	}
	if (m_changed.test(position)) {
		m_changed.reset(position);
		m_labelPoint = get_Position()
			+ (get_Size() - m_labelVertex) / 2.
			+ 0.7 * get_Palette()
				.get_Atlas()
					.MapSize(
						Atlas::Specification::Plug::Indicator::Tip::X - Atlas::Specification::Plug::Indicator::Start::X,
						0.F);
	}
	if (m_changed.test(label)) {
		m_changed.reset(label);
		m_labelMaskPoint = get_Palette()
			.get_Atlas()
				.MapPosition(
					Atlas::Specification::Plug::Label::Strip::Point::X + m_label % Atlas::Specification::Width,
					Atlas::Specification::Plug::Label::Strip::Point::Y + m_label / Atlas::Specification::Width);
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

	FillOpacityMask(
		m_color,
		m_labelPoint,
		m_labelVertex,
		m_labelMaskPoint);
}