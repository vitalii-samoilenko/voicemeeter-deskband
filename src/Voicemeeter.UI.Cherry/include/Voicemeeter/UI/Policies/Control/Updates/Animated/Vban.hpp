#ifndef VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_ANIMATED_VBAN_HPP
#define VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_ANIMATED_VBAN_HPP

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Glyph {
				namespace Updates {
					namespace Animated {
						template<typename TVban>
						struct Vban {
							inline void operator()(TVban &glyph, int state) const {
								glyph.set_AnimationVelocitySize(::std::valarray<int>{
									state == 0 ? -1 : 1
								});
							};
						};

						template<typename TPalette, typename TVban>
						class VbanFrame {
						public:
							inline explicit VbanFrame(TPalette &palette)
								: _palette{ palette } {

							};
							VbanFrame() = delete;
							VbanFrame(VbanFrame const &) = delete;
							inline VbanFrame(VbanFrame &&) = default;

							inline ~VbanFrame() = default;

							VbanFrame & operator=(VbanFrame const &) = delete;
							VbanFrame & operator=(VbanFrame &&) = delete;

							inline void operator()(TVban glyph) const {
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
									.Information, normalizedAnimationVertex[active]);
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
