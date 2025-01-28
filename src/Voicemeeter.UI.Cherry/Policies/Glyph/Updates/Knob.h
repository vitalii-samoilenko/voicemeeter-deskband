#pragma once

#include <type_traits>

#include "../../../Graphics/Bundles/Knob.h"
#include "../../../Graphics/Glyph.h"
#include "../../../Policies/State/Changes/Knob.h"
#include "../../../States/Knob.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						template<
							typename TInstrumentation,
							typename TBundle,
							typename TGlyph>
						struct Knob {
							static_assert(
								::std::is_base_of_v<Graphics::Bundles::Knob<TInstrumentation>, TBundle>,
								"TBundle must be derived from Knob");
							static_assert(
								::std::is_base_of_v<Graphics::Glyph<TInstrumentation, TBundle>, TGlyph>,
								"TGlyph must be derived from Glyph");

							inline void operator()(TGlyph& glyph, const States::Knob& state) const {
								glyph.get_Bundle()
									.set_Label(state.hold
										? Policies::State::Changes::Knob::ToLabel(state.gain)
										: Policies::State::Changes::Knob::ToLabel(state.id));
								int level{
									(state.level.size()
										? state.level.max()
										: 0)
								};
								const ::std::valarray<double>& color{
									(state.toggle
										? glyph.get_Bundle()
											.get_Palette()
												.get_Theme()
													.Warning
										: state.hold
											? Policies::State::Changes::Knob::DEFAULT < state.gain
												? glyph.get_Bundle()
													.get_Palette()
														.get_Theme()
															.Danger
												: glyph.get_Bundle()
													.get_Palette()
														.get_Theme()
															.PrimaryActive
											: level < 5
												? glyph.get_Bundle()
													.get_Palette()
														.get_Theme()
															.Inactive
												: level < 700
													? glyph.get_Bundle()
														.get_Palette()
															.get_Theme()
																.EqualizerLow
													: level < 10000
														? glyph.get_Bundle()
															.get_Palette()
																.get_Theme()
																	.EqualizerMedium
														: glyph.get_Bundle()
															.get_Palette()
																.get_Theme()
																	.EqualizerHigh)
								};
								glyph.get_Bundle()
									.set_FrameColor(color);
								glyph.get_Bundle()
									.set_LabelColor(color);
								glyph.get_Bundle()
									.set_Angle(state.gain);
								glyph.get_Bundle()
									.get_Palette()
										.get_Queue() += glyph.get_Bundle();
							};
						};
					}
				}
			}
		}
	}
}