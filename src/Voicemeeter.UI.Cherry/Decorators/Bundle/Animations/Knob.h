#pragma once

#include <type_traits>
#include <utility>

#include "../../../Graphics/Bundles/Knob.h"
#include "../Animation.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Decorators {
				namespace Bundle {
					namespace Animations {
						template<
							typename TInstrumentation,
							typename TBundle>
						class Knob : public Animation<TInstrumentation, TBundle> {
							using Animation = Animation<TInstrumentation, TBundle>;
							using Bundle = Graphics::Bundles::Knob<TInstrumentation>;
							using Palette = Graphics::Palette<TInstrumentation>;

							static_assert(
								::std::is_base_of_v<Bundle, TBundle>,
								"TBundle must be derived from Knob");

						public:
							enum animation_vector : size_t {
								level_low = 0, level_medium = 1, level_high = 2,
								gain_danger = 3,
								hold = 4, toggle = 5,
								label = 6
							};

							template<typename... Args>
							explicit Knob(
								Args&& ...args
							) : Animation{ {
									200LL * 1000LL * 1000LL, 200LL * 1000LL * 1000LL, 200LL * 1000LL * 1000LL,
									200LL * 1000LL * 1000LL,
									200LL * 1000LL * 1000LL, 200LL * 1000LL * 1000LL,
									200LL * 1000LL * 1000LL
								}, ::std::forward<Args>(args)... }
							  , m_to{} {

							};
							Knob() = delete;
							Knob(const Knob&) = delete;
							Knob(Knob&&) = delete;

							~Knob() = default;

							Knob& operator=(const Knob&) = delete;
							Knob& operator=(Knob&&) = delete;

							inline void set_To(size_t value) {
								m_to = value;
							};

						protected:
							virtual void OnFrame() override {
								using RGBA = Graphics::Theme::RGBA;

								::std::valarray<long long>& velocity{ Animation::get_Velocity() };
								const ::std::valarray<long long>& vertex{ Animation::get_AnimationSize() };
								const ::std::valarray<long long>& baseVertex{ Animation::get_AnimationBaseSize() };
								::std::valarray<double> result{
									TBundle::get_Palette()
										.get_Theme()
											.Inactive
								};
								Palette::Blend(result,
									TBundle::get_Palette()
										.get_Theme()
											.EqualizerLow,
									static_cast<double>(vertex[level_low]) / baseVertex[level_low]);
								Palette::Blend(result,
									TBundle::get_Palette()
										.get_Theme()
											.EqualizerMedium,
									static_cast<double>(vertex[level_medium]) / baseVertex[level_medium]);
								Palette::Blend(result,
									TBundle::get_Palette()
										.get_Theme()
											.EqualizerHigh,
									static_cast<double>(vertex[level_high]) / baseVertex[level_high]);
								::std::valarray<double> gain{
									TBundle::get_Palette()
										.get_Theme()
											.PrimaryActive
								};
								Palette::Blend(gain,
									TBundle::get_Palette()
										.get_Theme()
											.Danger,
									static_cast<double>(vertex[gain_danger]) / baseVertex[gain_danger]);
								Palette::Blend(result,
									gain,
									static_cast<double>(vertex[hold]) / baseVertex[hold]);
								Palette::Blend(result,
									TBundle::get_Palette()
										.get_Theme()
											.Warning,
									static_cast<double>(vertex[toggle]) / baseVertex[toggle]);
								TBundle::set_FrameColor(result);
								long long mid{ baseVertex[label] / 2 };
								result[RGBA::alpha] = ::std::abs(static_cast<double>(vertex[label] - mid) / mid);
								TBundle::set_LabelColor(result);
								if (0 < velocity[label] * (vertex[label] - mid)) {
									TBundle::set_Label(m_to);
								}
								velocity[vertex == 0LL || vertex == baseVertex] = 0LL;
							};

						private:
							size_t m_to;
						};
					}
				}
			}
		}
	}
}