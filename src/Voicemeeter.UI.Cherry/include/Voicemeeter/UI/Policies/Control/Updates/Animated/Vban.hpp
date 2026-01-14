#ifndef VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_VBAN_HPP
#define VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_VBAN_HPP

#include <cmath>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Control {
				namespace Updates {
					namespace Animated {
						template<typename TToolkit>
						struct VbanContext {
							typename TToolkit::Palette::gradient path;
							int distance2;
						};

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

							inline void operator()(
								TVban &control, typename TVban::state_t state) const {
								constexpr num_t ANIMATION_LENGTH{ 200 };
								vector_t targetVertex{ 0 };
								vector_t targetRgba{
									_toolkit.get_Theme()
										.Inactive
								};
								if (state) {
									targetVertex[0] = ANIMATION_LENGTH;
									targetRgba = _toolkit.get_Theme()
										.Information;
								}
								control.set_AnimationSize(targetVertex);
								auto animationVertex = control.get_AnimationSize()
									- control.get_AnimationPoint();
								control.set_AnimationContext(
									typename TVban::context_t{
										_toolkit.get_Palette()
											.Interpolate(targetRgba, control.get_FrameColor()),
										(animationVertex * animationVertex)
											.sum()
									});
							};
						
						private:
							TToolkit &_toolkit;
						};

						template<typename TToolkit, typename TVban>
						class VbanFrame {
						public:
							inline explicit VbanFrame(TToolkit &toolkit)
								: _toolkit{ toolkit } {

							};
							VbanFrame() = delete;
							VbanFrame(VbanFrame const &) = delete;
							inline VbanFrame(VbanFrame &&) = default;

							inline ~VbanFrame() = default;

							VbanFrame & operator=(VbanFrame const &) = delete;
							VbanFrame & operator=(VbanFrame &&) = delete;

							inline void operator()(TVban &control) const {
								auto animationVertex = control.get_AnimationSize()
									- control.get_AnimationPoint();
								num_t remaining2{
									(animationVertex * animationVertex)
										.sum()
								};
								typename TVban::context_t const &context{
									control.get_AnimationContext()
								};
								num_t rI{
									static_cast<num_t>(::std::sqrt(
										context.distance2 * SCALING_FACTOR / remaining2
										* SCALING_FACTOR))
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
