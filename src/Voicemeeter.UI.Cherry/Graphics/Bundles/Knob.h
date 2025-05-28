#pragma once

#include "../Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Graphics {
				namespace Bundles {
					template<typename TInstrumentation>
					class Knob : public Bundle<TInstrumentation> {
						using Palette = Palette<TInstrumentation>;
						using Bundle = Bundle<TInstrumentation>;
						using Atlas = Atlas<TInstrumentation>;

					public:
						inline Knob(
							Palette& palette,
							size_t label
						) : Bundle{ palette, { 2. * Atlas::Specification::Knob::Frame::Radius, 2. * Atlas::Specification::Knob::Frame::Radius } }
						  , m_label{ label }
						  , m_frameColor{
								Bundle::get_Palette()
									.get_Theme()
										.Inactive
							}
						  , m_labelColor{
								Bundle::get_Palette()
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

						};
						Knob() = delete;
						Knob(const Knob&) = delete;
						Knob(Knob&&) = delete;

						~Knob() = default;

						Knob& operator=(const Knob&) = delete;
						Knob& operator=(Knob&&) = delete;

						inline void set_Label(size_t value) {
							if (m_label == value) {
								return;
							}
							m_label = value;
							Bundle::get_Changed()
								.set(label);
						};
						inline void set_FrameColor(const ::std::valarray<double>& value) {
							m_frameColor = value;
						};
						inline void set_LabelColor(const ::std::valarray<double>& value) {
							m_labelColor = value;
						};
						inline void set_Angle(int value) {
							if (m_angle == value) {
								return;
							}
							m_angle = value;
							Bundle::get_Changed()
								.set(angle);
						};

						virtual void Execute() override {
							if (Bundle::get_Changed()
									.test(Bundle::size)) {
								Bundle::get_Changed()
									.reset(Bundle::size);
								m_maskPoint = Bundle::get_Palette()
									.get_Atlas()
										.MapPosition(
											Atlas::Specification::Knob::Frame::Point::X,
											Atlas::Specification::Knob::Frame::Point::Y);
								m_labelVertex = Bundle::get_Palette()
									.get_Atlas()
										.MapSize(Atlas::Specification::Block::Width, Atlas::Specification::Block::Height);
								Bundle::get_Changed()
									.set(Bundle::position);
								Bundle::get_Changed()
									.set(label);
								m_indicatorVertex = Bundle::get_Palette()
									.get_Atlas()
										.MapSize(
											2.F * Atlas::Specification::Knob::Indicator::Radius,
											2.F * Atlas::Specification::Knob::Indicator::Radius);
								m_indicatorMaskPoint = Bundle::get_Palette()
									.get_Atlas()
										.MapPosition(
											Atlas::Specification::Knob::Indicator::Point::X,
											Atlas::Specification::Knob::Indicator::Point::Y);
							}
							if (Bundle::get_Changed()
									.test(Bundle::position)) {
								Bundle::get_Changed()
									.reset(Bundle::position);
								m_labelPoint = Bundle::get_Position()
									+ (Bundle::get_Size() - m_labelVertex) / 2.;
								Bundle::get_Changed()
									.set(angle);
							}
							if (Bundle::get_Changed()
									.test(label)) {
								Bundle::get_Changed()
									.reset(label);
								m_labelMaskPoint = Bundle::get_Palette()
									.get_Atlas()
										.MapPosition(
											Atlas::Specification::Knob::Label::Strip::Point::X + m_label % Atlas::Specification::Width,
											Atlas::Specification::Knob::Label::Strip::Point::Y + m_label / Atlas::Specification::Width);
							}
							if (Bundle::get_Changed()
									.test(angle)) {
								Bundle::get_Changed()
									.reset(angle);
								constexpr FLOAT R{
									Atlas::Specification::Knob::Frame::Radius
									- Atlas::Specification::Knob::Frame::Stroke
									- 2.F * Atlas::Specification::Knob::Indicator::Radius
								};
								FLOAT phi{ m_angle / 18000.F * static_cast<FLOAT>(M_PI) };
								m_indicatorPoint = Bundle::get_Position()
									+ Bundle::get_Palette()
										.get_Atlas()
											.MapSize(
												R * ::std::cos(phi) + Atlas::Specification::Knob::Frame::Radius - Atlas::Specification::Knob::Indicator::Radius,
												R * ::std::sin(phi) + Atlas::Specification::Knob::Frame::Radius - Atlas::Specification::Knob::Indicator::Radius);
							}

							Bundle::get_Palette()
								.get_Atlas()
									.Fill(
										Bundle::get_Position(),
										Bundle::get_Size(),
										m_maskPoint,
										{ 2. * Atlas::Specification::Knob::Frame::Radius, 2. * Atlas::Specification::Knob::Frame::Radius },
										m_frameColor,
										false
									);
							//Bundle::get_Palette()
							//	.get_Atlas()
							//		.Fill(
							//			m_labelPoint,
							//			m_labelVertex,
							//			m_labelMaskPoint,
							//			m_labelVertex,
							//			m_labelColor,
							//			true
							//		);
							//Bundle::get_Palette()
							//	.get_Atlas()
							//		.Fill(
							//			m_indicatorPoint,
							//			m_indicatorVertex,
							//			m_indicatorMaskPoint,
							//			m_indicatorVertex,
							//			Bundle::get_Palette()
							//				.get_Theme()
							//					.Indicator,
							//			true
							//		);
						};

					private:
						enum knob_flags : size_t {
							label = Bundle::size + 1,
							angle = Bundle::size + 2
						};

						size_t m_label;
						::std::valarray<double> m_frameColor;
						::std::valarray<double> m_labelColor;
						int m_angle;
						::std::valarray<double> m_maskPoint;
						::std::valarray<double> m_labelPoint;
						::std::valarray<double> m_labelVertex;
						::std::valarray<double> m_labelMaskPoint;
						::std::valarray<double> m_indicatorPoint;
						::std::valarray<double> m_indicatorVertex;
						::std::valarray<double> m_indicatorMaskPoint;
					};
				}
			}
		}
	}
}