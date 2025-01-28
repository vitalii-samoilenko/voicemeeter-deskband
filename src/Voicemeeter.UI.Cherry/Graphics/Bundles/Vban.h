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
								get_Palette()
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
							if (m_changed.test(size)) {
								m_changed.reset(size);
								m_maskPoint = get_Palette()
									.get_Atlas()
										.MapPosition(
											Atlas::Specification::Vban::Frame::Point::X,
											Atlas::Specification::Vban::Frame::Point::Y);
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