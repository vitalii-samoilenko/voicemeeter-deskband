#ifndef VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_STRIPKNOB_HPP
#define VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_STRIPKNOB_HPP

#include <cmath>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Control {
				namespace Updates {
					namespace Animated {
						template<typename TToolkit>
						struct StripKnobContext {
							typename TToolkit::Palette::gradient path;
							int distance2;
						};

						template<typename TToolkit, typename TStripKnob>
						class StripKnob {
						public:
							inline explicit StripKnob(TToolkit &toolkit)
								: _toolkit{ toolkit } {

							};
							StripKnob() = delete;
							StripKnob(StripKnob const &) = delete;
							inline StripKnob(StripKnob &&) = default;

							inline ~StripKnob() = default;

							StripKnob & operator=(StripKnob const &) = delete;
							StripKnob & operator=(StripKnob &&) = delete;

							inline void operator()(
								TStripKnob &control, typename TStripKnob::state_t const &state) const {
								constexpr num_t ANIMATION_LENGTH{ 200 };
								vector_t targetVertex{
									0, 0, 0,
									0, 0,
									0
								};
								vector_t targetRgba{
									_toolkit.get_Theme()
										.Inactive
								};
								if (state.toggle) {
									targetVertex[5] = ANIMATION_LENGTH;
									targetRgba = _toolkit.get_Theme()
										.Warning;
								} else if (state.hold) {
									if (0 < state.degree) {
										targetVertex[4] = ANIMATION_LENGTH;
										targetRgba = _toolkit.get_Theme()
											.Error;
									} else {
										targetVertex[3] = ANIMATION_LENGTH;
										targetRgba = _toolkit.get_Theme()
											.Ok;
									}
								} else if (5 < state.level) {
									if (700 < state.level) {
										if (1000 < state.level) {
											targetVertex[2] = ANIMATION_LENGTH;
											targetRgba = _toolkit.get_Theme()
												.EqHigh;
										} else {
											targetVertex[1] = ANIMATION_LENGTH;
											targetRgba = _toolkit.get_Theme()
												.EqNormal;
										}
									} else {
										targetVertex[0] = ANIMATION_LENGTH;
										targetRgba = _toolkit.get_Theme()
											.EqLow;
									}
								}
								control.set_AnimationSize(targetVertex);
								control.set_IndicatorAngle(state.degree);
								auto animationVertex = control.get_AnimationSize()
									- control.get_AnimationPoint();
								control.set_AnimationContext(
									typename TStripKnob::context_t{
										_toolkit.get_Palette()
											.Interpolate(targetRgba, control.get_FrameColor()),
										(animationVertex * animationVertex)
											.sum()
									});
							};

						private:
							TToolkit &_toolkit;
						};

						template<typename TPalette, typename TStripKnob>
						class StripKnobFrame {
						public:
							inline explicit StripKnobFrame(TPalette &palette)
								: _palette{ palette } {

							};
							StripKnobFrame() = delete;
							StripKnobFrame(StripKnobFrame const &) = delete;
							inline StripKnobFrame(StripKnobFrame &&) = default;

							inline ~StripKnobFrame() = default;

							StripKnobFrame & operator=(StripKnobFrame const &) = delete;
							StripKnobFrame & operator=(StripKnobFrame &&) = delete;

							inline void operator()(TStripKnob &control) const {
								auto animationVertex = control.get_AnimationSize()
									- control.get_AnimationPoint();
								num_t remaining2{
									(animationVertex * animationVertex)
										.sum()
								};
								typename TStripKnob::context_t const &context{
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
							TPalette &_palette;
						};
					}
				}
			}
		}
	}
}

#endif
