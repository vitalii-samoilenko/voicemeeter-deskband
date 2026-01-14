#ifndef VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_VBAN_HPP
#define VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_VBAN_HPP

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Glyph {
				namespace Updates {
					template<typename TToolkit, typename TVban>
					class Vban {
					public:
						inline explicit Vban(TToolkit &toolkit)
							: _toolkit{ toolkit } {

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
									? _toolkit.get_Theme()
										.Inactive
									: _toolkit.get_Theme()
										.Information
							};
							glyph.set_FrameColor(rgba);
						};

					private:
						TToolkit &_toolkit;
					};
				}
			}
		}
	}
}

#endif
