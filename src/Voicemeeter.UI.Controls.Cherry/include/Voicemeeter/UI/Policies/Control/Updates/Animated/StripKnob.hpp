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
							inline void operator()(TStripKnob *that, state_t const &state) const {
								constexpr num_t AnimationLength{ 200 };
								vector_t targetVertex{
									0, 0, 0,
									0, 0,
									0
								};
								vector_t const *targetRgba{
									&_toolkit.get_Theme()
										.Inactive
								};
								if (state.toggle) {
									targetVertex[5] = AnimationLength;
									targetRgba = &_toolkit.get_Theme()
										.Warning;
								} else if (state.hold) {
									if (0 < state.degree) {
										targetVertex[4] = AnimationLength;
										targetRgba = &_toolkit.get_Theme()
											.Error;
									} else {
										targetVertex[3] = AnimationLength;
										targetRgba = &_toolkit.get_Theme()
											.Active;
									}
								} else if (5 < state.level) {
									if (700 < state.level) {
										if (1000 < state.level) {
											targetVertex[2] = AnimationLength;
											targetRgba = &_toolkit.get_Theme()
												.EqHigh;
										} else {
											targetVertex[1] = AnimationLength;
											targetRgba = &_toolkit.get_Theme()
												.EqMedium;
										}
									} else {
										targetVertex[0] = AnimationLength;
										targetRgba = &_toolkit.get_Theme()
											.EqLow;
									}
								}
								that->set_AnimationSize(targetVertex);
								that->set_IndicatorAngle(state.degree);
								that->set_Label(state.hold
									? 8 + floor(ans(state.degree * 4 / 15))
									: state.target);
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
							inline void operator()(TStripKnob *that) const {
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
								vector_t targetRgba{ context.path.pick(rI) };
								that->set_FrameColor(targetRgba);
								that->set_LabelColor(targetRgba);
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
