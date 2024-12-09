#pragma once

#include <string>
#include <type_traits>
#include <utility>

#include "Voicemeeter.UI/States/Knob.h"

#include "../../../Graphics/Glyphs/Knob.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						template<typename TKnob>
						class Knob : public TKnob {
							static_assert(
								::std::is_base_of_v<Graphics::Glyphs::Knob, TKnob>,
								"TKnob must be derived from Knob");

						public:
							template<typename... Args>
							explicit Knob(
								const ::std::wstring& label,
								Args&& ...args
							) : TKnob{ ::std::forward<Args>(args)... }
							  , m_label{ label } {
								TKnob::set_Label(m_label);
								const ::D2D1::ColorF& color{
									TKnob::get_Palette()
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
								TKnob::set_Label((state.hold
									? ::std::to_wstring(::std::abs(
										static_cast<int>(
											::std::floor((state.gain - 9000) / 375.))))
									: m_label));
								int level{
									(state.level.size()
										? state.level.max()
										: 0)
								};
								const ::D2D1::ColorF& color{
									(state.toggle
										? TKnob::get_Palette()
											.get_Theme()
												.Warning
										: state.hold
											? 9000 < state.gain
												? TKnob::get_Palette()
													.get_Theme()
														.Danger
												: TKnob::get_Palette()
													.get_Theme()
														.PrimaryActive
											: level < 5
												? TKnob::get_Palette()
													.get_Theme()
														.Inactive
												: level < 700
													? TKnob::get_Palette()
														.get_Theme()
															.EqualizerLow
													: level < 10000
														? TKnob::get_Palette()
															.get_Theme()
																.EqualizerMedium
														: TKnob::get_Palette()
															.get_Theme()
																.EqualizerHigh)
								};
								TKnob::set_FrameColor(color);
								TKnob::set_LabelColor(color);
								TKnob::set_Angle(state.gain / 100.F);
								TKnob::get_DirtyTracker()
									.set_Dirty(*this, true);
							};

						private:
							const ::std::wstring m_label;
						};
					}
				}
			}
		}
	}
}