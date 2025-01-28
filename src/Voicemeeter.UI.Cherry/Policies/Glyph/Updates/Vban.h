#pragma once

#include <type_traits>

#include "../../../Graphics/Bundles/Vban.h"
#include "../../../Graphics/Glyph.h"

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
						struct Vban {
							static_assert(
								::std::is_base_of_v<Graphics::Bundles::Vban<TInstrumentation>, TBundle>,
								"TBundle must be derived from VBan");
							static_assert(
								::std::is_base_of_v<Graphics::Glyph<TInstrumentation, TBundle>, TGlyph>,
								"TGlyph must be derived from Glyph");

							inline void operator()(TGlyph& glyph, const int& state) const {
								glyph.get_Bundle()
									.set_Color((state
										? glyph.get_Bundle()
											.get_Palette()
												.get_Theme()
													.SecondaryActive
										: glyph.get_Bundle()
											.get_Palette()
												.get_Theme()
													.Inactive));
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