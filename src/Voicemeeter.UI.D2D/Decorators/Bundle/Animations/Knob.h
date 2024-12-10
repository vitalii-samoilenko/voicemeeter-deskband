#pragma once

#include <type_traits>
#include <utility>

#include "../../../Graphics/Bundles/Knob.h"
#include "../Animation.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				namespace Bundle {
					namespace Animations {
						template<typename TBundle>
						class Knob : public Animation<TBundle> {
							static_assert(
								::std::is_base_of_v<Graphics::Bundles::Knob, TBundle>,
								"TBundle must be derived from Knob");

							enum animation_vector : size_t {
								level_low = 0, level_medium = 1, level_high = 2,
								gain_danger = 3,
								hold = 4, toggle = 5,
								label = 6
							};

							using Base = Animation<TBundle>;

						public:
							template<typename... Args>
							explicit Knob(
								Args&& ...args
							) : Base{ {
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

							inline void set_To(const ::std::wstring& value) {
								m_to = value;
							};

						protected:
							virtual void OnFrame() override {
								const ::std::valarray<long long>& velocity{ Base::get_Velocity() };
								const ::std::valarray<long long>& vertex{ Base::get_AnimationSize() };
								const ::std::valarray<long long>& baseVertex{ Base::get_AnimationBaseSize() };
								::D2D1::ColorF result{
									TBundle::get_Palette()
										.get_Theme()
											.Inactive
								};
								Graphics::Palette::Blend(result,
									TBundle::get_Palette()
										.get_Theme()
											.EqualizerLow,
									static_cast<FLOAT>(vertex[level_low]) / baseVertex[level_low]);
								Graphics::Palette::Blend(result,
									TBundle::get_Palette()
										.get_Theme()
											.EqualizerMedium,
									static_cast<FLOAT>(vertex[level_medium]) / baseVertex[level_medium]);
								Graphics::Palette::Blend(result,
									TBundle::get_Palette()
										.get_Theme()
											.EqualizerHigh,
									static_cast<FLOAT>(vertex[level_high]) / baseVertex[level_high]);
								::D2D1::ColorF gain{
									TBundle::get_Palette()
										.get_Theme()
											.PrimaryActive
								};
								Graphics::Palette::Blend(gain,
									TBundle::get_Palette()
										.get_Theme()
											.Danger,
									static_cast<FLOAT>(vertex[gain_danger]) / baseVertex[gain_danger]);
								Graphics::Palette::Blend(result, gain,
									static_cast<FLOAT>(vertex[hold]) / baseVertex[hold]);
								Graphics::Palette::Blend(result,
									TBundle::get_Palette()
										.get_Theme()
											.Warning,
									static_cast<FLOAT>(vertex[toggle]) / baseVertex[toggle]);
								TBundle::set_FrameColor(result);
								long long mid{ baseVertex[label] / 2 };
								result.a = ::std::abs(static_cast<FLOAT>(vertex[label] - mid) / mid);
								TBundle::set_LabelColor(result);
								if (0 < velocity[label] * (vertex[label] - mid)) {
									TBundle::set_Label(m_to);
								}
							};

						private:
							::std::wstring m_to;
						};
					}
				}
			}
		}
	}
}