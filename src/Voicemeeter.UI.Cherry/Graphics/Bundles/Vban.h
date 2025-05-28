#pragma once

#include "../Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Graphics {
				namespace Bundles {
					template<typename TInstrumentation>
					class Vban : public Bundle<TInstrumentation> {
						using Palette = Palette<TInstrumentation>;
						using Bundle = Bundle<TInstrumentation>;
						using Atlas = Atlas<TInstrumentation>;

					public:
						inline explicit Vban(
							Palette& palette
						) : Bundle{ palette, { Atlas::Specification::Vban::Frame::Width, Atlas::Specification::Vban::Frame::Height } }
						  , m_color{
								Bundle::get_Palette()
									.get_Theme()
										.Inactive
							}
						  , m_maskPoint{ 0., 0. } {

						};
						Vban() = delete;
						Vban(const Vban&) = delete;
						Vban(Vban&&) = delete;

						~Vban() = default;

						Vban& operator=(const Vban&) = delete;
						Vban& operator=(Vban&&) = delete;

						inline void set_Color(const ::std::valarray<double>& value) {
							m_color = value;
						};

						virtual void Execute() override {
							if (Bundle::get_Changed()
									.test(Bundle::size)) {
								Bundle::get_Changed()
									.reset(Bundle::size);
								m_maskPoint = Bundle::get_Palette()
									.get_Atlas()
										.MapPosition(
											Atlas::Specification::Vban::Frame::Point::X,
											Atlas::Specification::Vban::Frame::Point::Y);
							}

							constexpr FLOAT Scale{ 2.F * Atlas::Specification::Knob::Frame::Radius / Atlas::Specification::Vban::Frame::Height };
							Bundle::get_Palette()
								.get_Atlas()
									.Fill(
										Bundle::get_Position(),
										Bundle::get_Size(),
										m_maskPoint,
										{ static_cast<double>(Atlas::Specification::Vban::Frame::Width * Scale), static_cast<double>(Atlas::Specification::Vban::Frame::Height * Scale) },
										m_color,
										false
									);
						};

					private:
						::std::valarray<double> m_color;
						::std::valarray<double> m_maskPoint;
					};
				}
			}
		}
	}
}