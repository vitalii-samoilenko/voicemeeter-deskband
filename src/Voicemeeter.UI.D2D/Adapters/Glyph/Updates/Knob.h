#pragma once

#include <type_traits>

#include "Voicemeeter.UI/States/Knob.h"

#include "../../../Graphics/Bundles/Knob.h"
#include "../../../Graphics/Glyph.h"
#include "../../../Policies/State/Changes/Knob.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						template<typename TBundle, typename TGlyph>
						class Knob : public TGlyph {
							static_assert(
								::std::is_base_of_v<Graphics::Bundles::Knob, TBundle>,
								"TBundle must be derived from Knob");
							static_assert(
								::std::is_base_of_v<Graphics::Glyph<TBundle>, TGlyph>,
								"TGlyph must be derived from Glyph");

						public:
							using TGlyph::TGlyph;

							Knob() = delete;
							Knob(const Knob&) = delete;
							Knob(Knob&&) = delete;

							~Knob() = default;

							Knob& operator=(const Knob&) = delete;
							Knob& operator=(Knob&&) = delete;

							inline void Update(const States::Knob& state) {
								TGlyph::get_Bundle()
									.set_Label(state.hold
										? Policies::State::Changes::Knob::ToLabel(state.gain)
										: Policies::State::Changes::Knob::ToLabel(state.id));
								int level{
									(state.level.size()
										? state.level.max()
										: 0)
								};
								const ::D2D1::ColorF& color{
									(state.toggle
										? TGlyph::get_Bundle()
											.get_Palette()
												.get_Theme()
													.Warning
										: state.hold
											? 9000 < state.gain
												? TGlyph::get_Bundle()
													.get_Palette()
														.get_Theme()
															.Danger
												: TGlyph::get_Bundle()
													.get_Palette()
														.get_Theme()
															.PrimaryActive
											: level < 5
												? TGlyph::get_Bundle()
													.get_Palette()
														.get_Theme()
															.Inactive
												: level < 700
													? TGlyph::get_Bundle()
														.get_Palette()
															.get_Theme()
																.EqualizerLow
													: level < 10000
														? TGlyph::get_Bundle()
															.get_Palette()
																.get_Theme()
																	.EqualizerMedium
														: TGlyph::get_Bundle()
															.get_Palette()
																.get_Theme()
																	.EqualizerHigh)
								};
								TGlyph::get_Bundle()
									.set_FrameColor(color);
								TGlyph::get_Bundle()
									.set_LabelColor(color);
								TGlyph::get_Bundle()
									.set_Angle(state.gain / 100.F);
								TGlyph::get_Bundle()
									.get_Palette()
										.Queue(TGlyph::get_Bundle());
							};
						};
					}
				}
			}
		}
	}
}