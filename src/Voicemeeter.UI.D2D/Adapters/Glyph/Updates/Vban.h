#pragma once

#include <type_traits>

#include "../../../Graphics/Bundles/Vban.h"
#include "../../../Graphics/Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						template<typename TBundle, typename TGlyph>
						class Vban : public TGlyph {
							static_assert(
								::std::is_base_of_v<Graphics::Bundles::Vban, TBundle>,
								"TBundle must be derived from VBan");
							static_assert(
								::std::is_base_of_v<Graphics::Glyph<TBundle>, TGlyph>,
								"TGlyph must be derived from Glyph");

						public:
							using TGlyph::TGlyph;

							Vban() = delete;
							Vban(const Vban&) = delete;
							Vban(Vban&&) = delete;

							~Vban() = default;

							Vban& operator=(const Vban&) = delete;
							Vban& operator=(Vban&&) = delete;

							inline void Update(const int& state) {
								TGlyph::get_Bundle()
									.set_Color((state
										? TGlyph::get_Bundle()
											.get_Palette()
												.get_Theme()
													.SecondaryActive
										: TGlyph::get_Bundle()
											.get_Palette()
												.get_Theme()
													.Inactive));
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