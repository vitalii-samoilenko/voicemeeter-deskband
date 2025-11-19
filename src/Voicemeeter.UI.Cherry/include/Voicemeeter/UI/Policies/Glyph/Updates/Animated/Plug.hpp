#ifndef VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_ANIMATED_PLUG_HPP
#define VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_ANIMATED_PLUG_HPP

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Glyph {
				namespace Updates {
					namespace Animated {
						template<typename TPlug>
						struct Plug {
							inline void operator()(TPlug &glyph, int state) const {
								glyph.set_AnimationVelocitySize(::std::valarray<int>{
									state == 0 ? -1 : 1
								});
							};
						};

						template<typename TPalette, typename TPlug>
						class PlugFrame {
						public:
							inline explicit PlugFrame(TPalette &palette)
								: _palette{ palette } {

							};
							PlugFrame() = delete;
							PlugFrame(PlugFrame const &) = delete;
							inline PlugFrame(PlugFrame &&) = default;

							inline ~PlugFrame() = default;

							PlugFrame & operator=(PlugFrame const &) = delete;
							PlugFrame & operator=(PlugFrame &&) = delete;

							inline void operator()(TPlug glyph) const {
								enum dimension : size_t {
									/* inactive */ active = 0
								};
								::std::valarray<int> normalizedAnimationVertex{
									(glyph.get_AnimationSize() * SCALING_FACTOR)
										/ glyph.get_AnimationBaseSize()
								};
								::std::valarray<int> rgba{
									_palette.get_Theme()
										.Inactive
								};
								_palette.Blend(rgba, _palette.get_Theme()
									.Active, normalizedAnimationVertex[active]);
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
}

#endif
