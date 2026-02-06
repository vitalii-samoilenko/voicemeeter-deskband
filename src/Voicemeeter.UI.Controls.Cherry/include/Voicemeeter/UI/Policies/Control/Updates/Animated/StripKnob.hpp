#ifndef VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_STRIPKNOB_HPP
#define VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_STRIPKNOB_HPP

#include "wheel.hpp"

#include "Voicemeeter/UI/States/StripKnob.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Control {
				namespace Updates {
					namespace Animated {
						template<typename TToolkit>
						struct StripKnobContext {
							typename TToolkit::Palette::gradient path;
							num_t distance2;
						};

						template<typename TToolkit>
						class StripKnob {
						public:
							using context_t = StripKnobContext<TToolkit>;
							using state_t = States::StripKnob;

							inline explicit StripKnob(TToolkit &toolkit)
								: _toolkit{ toolkit } {

							};
							StripKnob() = delete;
							StripKnob(StripKnob const &) = delete;
							inline StripKnob(StripKnob &&) = default;

							inline ~StripKnob() = default;

							StripKnob & operator=(StripKnob const &) = delete;
							StripKnob & operator=(StripKnob &&) = delete;

							template<typename TStripKnob>
							inline void operator()(TStripKnob &control, state_t const &state) const {
								constexpr num_t AnimationLength{ 200 };
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
									targetVertex[5] = AnimationLength;
									targetRgba = _toolkit.get_Theme()
										.Warning;
								} else if (state.hold) {
									if (0 < state.degree) {
										targetVertex[4] = AnimationLength;
										targetRgba = _toolkit.get_Theme()
											.Error;
									} else {
										targetVertex[3] = AnimationLength;
										targetRgba = _toolkit.get_Theme()
											.Active;
									}
								} else if (5 < state.level) {
									if (700 < state.level) {
										if (1000 < state.level) {
											targetVertex[2] = AnimationLength;
											targetRgba = _toolkit.get_Theme()
												.EqHigh;
										} else {
											targetVertex[1] = AnimationLength;
											targetRgba = _toolkit.get_Theme()
												.EqMedium;
										}
									} else {
										targetVertex[0] = AnimationLength;
										targetRgba = _toolkit.get_Theme()
											.EqLow;
									}
								}
								control.set_AnimationSize(targetVertex);
								control.set_IndicatorAngle(state.degree);
								auto animationVertex = control.get_AnimationSize()
									- control.get_AnimationPoint();
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
						class StripKnobFrame {
						public:
							using context_t = StripKnobContext<TToolkit>;

							inline explicit StripKnobFrame(TToolkit &toolkit)
								: _toolkit{ toolkit } {

							};
							StripKnobFrame() = delete;
							StripKnobFrame(StripKnobFrame const &) = delete;
							inline StripKnobFrame(StripKnobFrame &&) = default;

							inline ~StripKnobFrame() = default;

							StripKnobFrame & operator=(StripKnobFrame const &) = delete;
							StripKnobFrame & operator=(StripKnobFrame &&) = delete;

							template<typename TStripKnob>
							inline void operator()(TStripKnob &control) const {
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
