#pragma once

#include <type_traits>

#include "../../../Graphics/Bundles/Plug.h"
#include "../../../Graphics/Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						template<typename TBundle, typename TGlyph>
						class Plug : public TGlyph {
							static_assert(
								::std::is_base_of_v<Graphics::Bundles::Plug, TBundle>,
								"TBundle must be derived from Plug");
							static_assert(
								::std::is_base_of_v<Graphics::Glyph<TBundle>, TGlyph>,
								"TGlyph must be derived from Glyph");

						public:
							using TGlyph::TGlyph;

							Plug() = delete;
							Plug(const Plug&) = delete;
							Plug(Plug&&) = delete;

							~Plug() = default;

							Plug& operator=(const Plug&) = delete;
							Plug& operator=(Plug&&) = delete;

							inline void Update(const int& state) {
								TGlyph::get_Bundle()
									.set_Color((state
										? TGlyph::get_Bundle()
											.get_Palette()
												.get_Theme()
													.PrimaryActive
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