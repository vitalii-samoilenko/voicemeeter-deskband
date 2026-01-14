#ifndef VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_PLUG_HPP
#define VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_PLUG_HPP

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Glyph {
				namespace Updates {
					template<typename TToolkit, typename TPlug>
					class Plug {
					public:
						inline explicit Plug(TToolkit &toolkit)
							: _toolkit{ toolkit } {

						};
						Plug() = delete;
						Plug(Plug const &) = delete;
						inline Plug(Plug &&) = default;

						inline ~Plug() = default;

						Plug & operator=(Plug const &) = delete;
						Plug & operator=(Plug &&) = delete;

						inline void operator()(TPlug &glyph, int state) const {
							::std::valarray<int> const &rgba{
								state == 0
									? _toolkit.get_Theme()
										.Inactive
									: _toolkit.get_Theme()
										.Active
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
