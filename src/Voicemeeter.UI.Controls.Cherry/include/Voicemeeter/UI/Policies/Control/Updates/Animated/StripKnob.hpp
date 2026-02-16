#ifndef VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_STRIPKNOB_HPP
#define VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_STRIPKNOB_HPP

#include "wheel.hpp"

#include "Voicemeeter/UI/Layouts/Atlas.hpp"
#include "Voicemeeter/UI/States/StripKnob.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Control {
				namespace Updates {
					namespace Animated {
						constexpr num_t _StripKnob_AnimationLength{ 200 };

						template<typename TToolkit>
						struct StripKnobContext {
							typename TToolkit::Palette::gradient path;
							num_t distance2;
							size_t target;
							size_t gain;
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
								vector_t targetVertex{
									0, 0, 0,
									0, 0,
									0,
									0
								};
								vector_t const *targetRgba{
									&_toolkit.get_Theme()
										.Inactive
								};
								size_t gain{
									Layouts::Atlas::Label::Names
									+ pop(floor(ans(state.degree * 4 / 15)))
								};
								if (state.toggle) {
									targetVertex[5] = _StripKnob_AnimationLength;
									targetRgba = &_toolkit.get_Theme()
										.Warning;
								} else if (state.hold) {
									if (0 < state.degree) {
										targetVertex[4] = _StripKnob_AnimationLength;
										targetRgba = &_toolkit.get_Theme()
											.Error;
									} else {
										targetVertex[3] = _StripKnob_AnimationLength;
										targetRgba = &_toolkit.get_Theme()
											.Active;
									}
								} else if (5 < state.level) {
									if (700 < state.level) {
										if (1000 < state.level) {
											targetVertex[2] = _StripKnob_AnimationLength;
											targetRgba = &_toolkit.get_Theme()
												.EqHigh;
										} else {
											targetVertex[1] = _StripKnob_AnimationLength;
											targetRgba = &_toolkit.get_Theme()
												.EqMedium;
										}
									} else {
										targetVertex[0] = _StripKnob_AnimationLength;
										targetRgba = &_toolkit.get_Theme()
											.EqLow;
									}
								}
								if (state.hold) {
									targetVertex[6] = _StripKnob_AnimationLength;
								}
								that->set_AnimationSize(targetVertex);
								that->set_IndicatorAngle(state.degree);
								num_t shifted{ that->get_AnimationPosition()[6] - _StripKnob_AnimationLength / 2 };
								that->set_Label(shifted < 0
									? state.target
									: gain);
								auto animationVertex = that->get_AnimationSize()
									- that->get_AnimationPosition();
								auto animationVertex2 = animationVertex * animationVertex;
								that->set_AnimationContext(context_t{
									_toolkit.get_Palette()
										.Interpolate(that->get_FrameColor(), *targetRgba),
									sum(vector_t{ animationVertex2[slice_t{ 0, 6, 1 }] }),
									state.target, gain
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
								auto animationVertex2 = animationVertex * animationVertex;
								num_t remaining2{
									sum(vector_t{ animationVertex2[slice_t{ 0, 6, 1 }] })
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
								num_t shifted{ that->get_AnimationPosition()[6] - _StripKnob_AnimationLength / 2 };
								that->set_Label(shifted < 0
									? context.target
									: context.gain);
								targetRgba[3] = push(255) * ans(shifted) / (_StripKnob_AnimationLength / 2);
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
