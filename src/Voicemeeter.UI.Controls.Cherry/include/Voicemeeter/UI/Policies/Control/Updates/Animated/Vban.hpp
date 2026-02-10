#ifndef VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_VBAN_HPP
#define VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_VBAN_HPP

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Control {
				namespace Updates {
					namespace Animated {
						template<typename TToolkit>
						struct VbanContext {
							typename TToolkit::Palette::gradient path;
							num_t distance2;
						};

						template<typename TToolkit>
						class Vban {
						public:
							using context_t = VbanContext<TToolkit>;
							using state_t = num_t;

							inline explicit Vban(TToolkit &toolkit)
								: _toolkit{ toolkit } {

							};
							Vban() = delete;
							Vban(Vban const &) = delete;
							inline Vban(Vban &&) = default;

							inline ~Vban() = default;

							Vban & operator=(Vban const &) = delete;
							Vban & operator=(Vban &&) = delete;

							template<typename TVban>
							inline void operator()(TVban *that, state_t state) const {
								constexpr num_t AnimationLength{ 200 };
								vector_t targetVertex{ 0 };
								vector_t const *targetRgba{
									&_toolkit.get_Theme()
										.Inactive
								};
								if (state) {
									targetVertex[0] = AnimationLength;
									targetRgba = &_toolkit.get_Theme()
										.Information;
								}
								that->set_AnimationSize(targetVertex);
								auto animationVertex = that->get_AnimationSize()
									- that->get_AnimationPosition();
								that->set_AnimationContext(context_t{
									_toolkit.get_Palette()
										.Interpolate(that->get_FrameColor(), *targetRgba),
									sum(animationVertex * animationVertex)
								});
							};
						
						private:
							TToolkit &_toolkit;
						};

						template<typename TToolkit>
						class VbanFrame {
						public:
							using context_t = VbanContext<TToolkit>;

							inline explicit VbanFrame(TToolkit &toolkit)
								: _toolkit{ toolkit } {

							};
							VbanFrame() = delete;
							VbanFrame(VbanFrame const &) = delete;
							inline VbanFrame(VbanFrame &&) = default;

							inline ~VbanFrame() = default;

							VbanFrame & operator=(VbanFrame const &) = delete;
							VbanFrame & operator=(VbanFrame &&) = delete;

							template<typename TVban>
							inline void operator()(TVban *that) const {
								auto animationVertex = that->get_AnimationSize()
									- that->get_AnimationPosition();
								num_t remaining2{
									sum(animationVertex * animationVertex)
								};
								context_t const &context{
									that->get_AnimationContext()
								};
								num_t rI{
									context.distance2
										? sqrt(
											push(context.distance2)
											/ (context.distance2 - remaining2)
										) : One
								};
								that->set_FrameColor(
									context.path.pick(rI));
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
