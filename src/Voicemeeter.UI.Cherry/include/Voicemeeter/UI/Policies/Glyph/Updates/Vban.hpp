#ifndef VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_VBAN_HPP
#define VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_VBAN_HPP

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Glyph {
				namespace Updates {
					template<typename TPalette, typename TVban>
					class Vban {
					public:
						inline explicit Vban(TPalette &palette)
							: _palette{ palette } {

						};
						Vban() = delete;
						Vban(Vban const &) = delete;
						inline Vban(Vban &&) = default;

						inline ~Vban() = default;

						Vban & operator=(Vban const &) = delete;
						Vban & operator=(Vban &&) = delete;

						inline void operator()(TVban &glyph, int state) const {
							::std::valarray<int> const &rgba{
								state == 0
									? _palette.get_Theme()
										.Inactive
									: _palette.get_Theme()
										.Information
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
