#pragma once

#include <cmath>
#include <string>
#include <type_traits>
#include <utility>

#include "Voicemeeter.UI/States/Knob.h"

#include "../../../../Graphics/Glyphs/Knob.h"
#include "../Animation.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						namespace Animations {
							template<typename TKnob>
							class Knob : public Animation<UI::Policies::Size::Scales::Stretch, TKnob, States::Knob> {
								static_assert(
									::std::is_base_of_v<Graphics::Glyphs::Knob, TKnob>,
									"TKnob must be derived from Knob");

								enum animation_vector : size_t {
									level_low = 0, level_medium = 1, level_high = 2,
									gain_danger = 3,
									hold = 4, toggle = 5,
									label = 6
								};

								using Animation = Animation<UI::Policies::Size::Scales::Stretch, TKnob, States::Knob>;

							public:
								template<typename... Args>
								explicit Knob(
									const ::std::wstring& label,
									Args&& ...args
								) : Animation{{
										200LL * 1000LL, 200LL * 1000LL, 200LL * 1000LL,
										200LL * 1000LL,
										200LL * 1000LL, 200LL * 1000LL,
										200LL * 1000LL
									}, ::std::forward<Args>(args)... }
								  , m_label{ label }
								  , m_gain{} {
									TKnob::set_Label(m_label);
									const ::D2D1::ColorF& color{
										TKnob::get_Canvas()
											.get_Palette()
												.get_Theme()
													.Inactive
									};
									TKnob::set_FrameColor(color);
									TKnob::set_LabelColor(color);
									TKnob::set_Angle(90.F);
								};
								Knob() = delete;
								Knob(const Knob&) = delete;
								Knob(Knob&&) = delete;

								~Knob() = default;

								Knob& operator=(const Knob&) = delete;
								Knob& operator=(Knob&&) = delete;

								inline void Update(const States::Knob& state) {
									::std::valarray<long long>& vertex{ Animation::get_Velocity() };
									if (state.hold) {
										vertex[hold] = 1;
										vertex[label] = 1;
									} else {
										vertex[hold] = -1;
										vertex[label] = -1;
									}
									vertex[toggle] = state.toggle
										? 1
										: -1;
									vertex[gain_danger] = 9000 < state.gain
										? 1
										: -1;
									int level{
										(state.level.size()
											? state.level.max()
											: 0)
									};
									if (level < 5) {
										vertex[level_low] = -1;
										vertex[level_medium] = -1;
										vertex[level_high] = -1;
									} else if (level < 700) {
										vertex[level_low] = 1;
										vertex[level_medium] = -1;
										vertex[level_high] = -1;
									} else if (level < 10000) {
										vertex[level_low] = 1;
										vertex[level_medium] = 1;
										vertex[level_high] = -1;
									} else {
										vertex[level_low] = 1;
										vertex[level_medium] = 1;
										vertex[level_high] = 1;
									}
									m_gain = ::std::to_wstring(::std::abs(
										static_cast<int>(
											::std::floor((state.gain - 9000) / 375.))));
									TKnob::set_Angle(state.gain / 100.F);
								};

							protected:
								virtual void OnFrame() override {
									auto blend = [](::D2D1::ColorF& dst, const ::D2D1::ColorF& src, FLOAT alpha)->void {
										dst.r = dst.r * (1.F - alpha) + src.r * alpha;
										dst.g = dst.g * (1.F - alpha) + src.g * alpha;
										dst.b = dst.b * (1.F - alpha) + src.b * alpha;
									};
									const ::std::valarray<long long>& vertex{ Animation::get_AnimationSize() };
									const ::std::valarray<long long>& baseVertex{ Animation::get_AnimationBaseSize() };
									::D2D1::ColorF result{
										TKnob::get_Canvas()
											.get_Palette()
												.get_Theme()
													.Inactive
									};
									blend(result,
										TKnob::get_Canvas()
											.get_Palette()
												.get_Theme()
													.EqualizerLow,
										static_cast<FLOAT>(vertex[level_low]) / baseVertex[level_low]);
									blend(result,
										TKnob::get_Canvas()
											.get_Palette()
												.get_Theme()
													.EqualizerMedium,
										static_cast<FLOAT>(vertex[level_medium]) / baseVertex[level_medium]);
									blend(result,
										TKnob::get_Canvas()
											.get_Palette()
												.get_Theme()
													.EqualizerHigh,
										static_cast<FLOAT>(vertex[level_high]) / baseVertex[level_high]);
									::D2D1::ColorF gain{
										TKnob::get_Canvas()
											.get_Palette()
												.get_Theme()
													.PrimaryActive
									};
									blend(gain,
										TKnob::get_Canvas()
											.get_Palette()
												.get_Theme()
													.Danger,
										static_cast<FLOAT>(vertex[gain_danger]) / baseVertex[gain_danger]);
									blend(result, gain,
										static_cast<FLOAT>(vertex[hold]) / baseVertex[hold]);
									blend(result,
										TKnob::get_Canvas()
											.get_Palette()
												.get_Theme()
													.Warning,
										static_cast<FLOAT>(vertex[toggle]) / baseVertex[toggle]);
									TKnob::set_FrameColor(result);
									long long mid{ baseVertex[label] / 2 };
									result.a = ::std::abs(static_cast<FLOAT>(vertex[label] - mid) / mid);
									TKnob::set_LabelColor(result);
									TKnob::set_Label((vertex[label] < mid
										? m_label
										: m_gain));
								};

							private:
								const ::std::wstring m_label;
								::std::wstring m_gain;
							};
						}
					}
				}
			}
		}
	}
}