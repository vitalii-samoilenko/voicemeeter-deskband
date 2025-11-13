#ifndef VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_PLUG_HPP
#define VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_PLUG_HPP

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Glyph {
				namespace Updates {
					template<typename TPalette>
					class Plug {
					public:
						inline explicit Plug(TPalette &palette)
							: _palette{ palette } {

						};
						Plug() = delete;
						Plug(Plug const &) = delete;
						inline Plug(Plug &&) = default;

						inline ~Plug() = default;

						Plug & operator=(Plug const &) = delete;
						Plug & operator=(Plug &&) = delete;

						template<typename TPlug>
						inline void operator()(TPlug &glyph, int state) const {
							::std::valarray<int> const &rgba{
								state == 0
									? _palette.get_Theme()
										.Inactive
									: _palette.get_Theme()
										.Active
							};
							glyph.set_Color(rgba);
							glyph.set_LabelColor(rgba);
						};
					private:
						TPalette &_palette;
					};
				}
			}
		}
	}
}

#endif
