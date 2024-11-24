#pragma once

#include <type_traits>
#include <utility>

#include "../../../Graphics/Glyphs/Vban.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						template<typename TVban>
						class Vban : TVban {
							static_assert(
								::std::is_base_of_v<Graphics::Glyphs::VBan, TVban>,
								"TVban must be derived from VBan");

						public:
							template<typename... Args>
							explicit Vban(
								Args&& ...args
							) : TVban{ ::std::forward<Args>(args)... } {
								TVban::set_Color(TVban::get_Canvas()
									.get_Palette()
										.get_Theme()
											.Inactive);
							};
							Vban() = delete;
							Vban(const Vban&) = delete;
							Vban(Vban&&) = delete;

							~Vban() = default;

							Vban& operator=(const Vban&) = delete;
							Vban& operator=(Vban&&) = delete;

							void Update(const int& state) {
								TVban::set_Color((state
									? TVban::get_Canvas()
										.get_Palette()
											.get_Theme()
												.SecondaryActive
									: TVban::get_Canvas()
										.get_Palette()
											.get_Theme()
												.Inactive));
							};
						};
					}
				}
			}
		}
	}
}