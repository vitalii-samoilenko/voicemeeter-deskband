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
							inline void operator()(TVban &control, state_t state) const {
								constexpr num_t AnimationLength{ 200 };
								vector_t targetVertex{ 0 };
								vector_t targetRgba{
									_toolkit.get_Theme()
										.Inactive
								};
								if (state) {
									targetVertex[0] = AnimationLength;
									targetRgba = _toolkit.get_Theme()
										.Information;
								}
								control.set_AnimationSize(targetVertex);
								auto animationVertex = control.get_AnimationSize()
									- control.get_AnimationPosition();
								control.set_AnimationContext(context_t{
									_toolkit.get_Palette()
										.Interpolate(targetRgba, control.get_FrameColor()),
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
							inline void operator()(TVban &control) const {
								auto animationVertex = control.get_AnimationSize()
									- control.get_AnimationPoint();
								num_t remaining2{
									sum(animationVertex * animationVertex)
								};
								context_t const &context{
									control.get_AnimationContext()
								};
								num_t rI{
										remaining2
											? sqrt(push(context.distance2) / remaining2)
											: One
								};
								control.set_FrameColor(
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
