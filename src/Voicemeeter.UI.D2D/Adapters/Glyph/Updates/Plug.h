#pragma once

#include <string>
#include <type_traits>
#include <utility>

#include "../../../Graphics/Glyphs/Plug.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						template<typename TPlug>
						class Plug : public TPlug {
							static_assert(
								::std::is_base_of_v<Graphics::Glyphs::Plug, TPlug>,
								"TPlug must be derived from Plug");

						public:
							template<typename... Args>
							explicit Plug(
								const ::std::wstring& label,
								Args&& ...args
							) : TPlug{ ::std::forward<Args>(args)... } {
								TPlug::set_Label(label);
								TPlug::set_Color(TPlug::get_Canvas()
									.get_Palette()
										.get_Theme()
											.Inactive);
							};
							Plug() = delete;
							Plug(const Plug&) = delete;
							Plug(Plug&&) = delete;

							~Plug() = default;

							Plug& operator=(const Plug&) = delete;
							Plug& operator=(Plug&&) = delete;

							void Update(const int& state) {
								TPlug::set_Color((state
									? TPlug::get_Canvas()
										.get_Palette()
											.get_Theme()
												.PrimaryActive
									: TPlug::get_Canvas()
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