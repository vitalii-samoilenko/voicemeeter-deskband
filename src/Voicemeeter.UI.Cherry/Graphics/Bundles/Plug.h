#pragma once

#include "../Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Graphics {
				namespace Bundles {
					template<typename TInstrumentation>
					class Plug : public Bundle<TInstrumentation> {
						using Palette = Palette<TInstrumentation>;
						using Bundle = Bundle<TInstrumentation>;
						using Atlas = Atlas<TInstrumentation>;

					public:
						inline Plug(
							Palette& palette,
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

						};
						Plug() = delete;
						Plug(const Plug&) = delete;
						Plug(Plug&&) = delete;

						~Plug() = default;

						Plug& operator=(const Plug&) = delete;
						Plug& operator=(Plug&&) = delete;

						inline void set_Label(size_t value) {
							if (m_label == value) {
								return;
							}
							m_label = value;
							m_changed.set(label);
						}
						inline void set_Color(const ::std::valarray<double>& value) {
							m_color = value;
						};

						virtual void Execute() override {
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
								.get_Atlas()
									.Fill(
										get_Position(),
										get_Size(),
										m_maskPoint,
										m_color,
										false
									);
							get_Palette()
								.get_Atlas()
									.Fill(
										m_labelPoint,
										m_labelVertex,
										m_labelMaskPoint,
										m_color,
										true
									);
						};

					private:
						enum plug_flags : size_t {
							label = size + 1
						};

						size_t m_label;
						::std::valarray<double> m_color;
						::std::valarray<double> m_maskPoint;
						::std::valarray<double> m_labelPoint;
						::std::valarray<double> m_labelVertex;
						::std::valarray<double> m_labelMaskPoint;
					};
				}
			}
		}
	}
}