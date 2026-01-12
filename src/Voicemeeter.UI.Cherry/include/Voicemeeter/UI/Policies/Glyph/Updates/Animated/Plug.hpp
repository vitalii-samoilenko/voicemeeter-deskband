#ifndef VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_ANIMATED_PLUG_HPP
#define VOICEMEETER_UI_POLICIES_GLYPH_UPDATES_ANIMATED_PLUG_HPP

#include <cmath>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Glyph {
				namespace Updates {
					namespace Animated {
						template<typename TToolkit>
						struct PlugContext {
							typename TToolkit::Palette::gradient path;
							int distanceQ
						};

						template<typename TToolkit, typename TPlug>
						struct Plug {
							inline void operator()(TPlug &glyph, int state) const {
								glyph.set_AnimationSize(
									::std::valarray<int>{ state * 200 });
								::std::valarray<int> animationVertex{
									glyph.get_AnimationSize()
									- glyph.get_AnimationPoint()
								};
								glyph.set_AnimationContext(
									PlugContext<TToolkit>{
										_toolkit.get_Palette()
											.Interpolate(
												glyph.get_FrameColor(),
												0 < state
													? _toolkit.get_Theme()
														.Active
													: _toolkit.get_Theme()
														.Inactive)),
										(animationVertex * animationVertex)
											.sum()
									});
							};
						};

						template<typename TToolkit, typename TPlug>
						class PlugFrame {
						public:
							inline explicit PlugFrame(TToolkit &toolkit)
								: _toolkit{ toolkit } {

							};
							PlugFrame() = delete;
							PlugFrame(PlugFrame const &) = delete;
							inline PlugFrame(PlugFrame &&) = default;

							inline ~PlugFrame() = default;

							PlugFrame & operator=(PlugFrame const &) = delete;
							PlugFrame & operator=(PlugFrame &&) = delete;

							inline void operator()(TPlug glyph) const {
								::std::valarray<int> animationVertex{
									glyph.get_AnimationSize()
									- glyph.get_AnimationPoint()
								};
								int remainingQ{
									(animationVertex * animationVertex)
										.sum()
								};
								PlugContext<TToolkit> const &context{
									glyph.get_AnimationContext()
								};
								int at{
									SCALING_FACTOR - ::std::sqrt(
										remainingQ * SCALING_FACTOR / context.distanceQ
										* SCALING_FACTOR)
								};
								glyph.set_FrameColor(
									context.path.pick(at));
							};

						private:
							TToolkit &_toolkit;
						};
					}
				}
			}
		}
	}
}

#endif
