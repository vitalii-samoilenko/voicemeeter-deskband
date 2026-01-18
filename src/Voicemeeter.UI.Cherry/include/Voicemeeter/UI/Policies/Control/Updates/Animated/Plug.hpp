#ifndef VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_PLUG_HPP
#define VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_PLUG_HPP

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Control {
				namespace Updates {
					namespace Animated {
						template<typename TToolkit>
						struct PlugContext {
							typename TToolkit::Palette::gradient path;
							num_t distance2;
						};

						template<typename TToolkit, typename TPlug>
						class Plug {
						public:
							inline explicit Plug(TToolkit &toolkit)
								: _toolkit{ toolkit } {

							};
							Plug() = delete;
							Plug(Plug const &) = delete;
							inline Plug(Plug &&) = default;

							inline ~Plug() = default;

							Plug & operator=(Plug const &) = delete;
							Plug & operator=(Plug &&) = delete;

							inline void operator()(
								TPlug &control, typename TPlug::state_t state) const {
								constexpr num_t AnimationLength{ 200 };
								vector_t targetVertex{ 0 };
								vector_t targetRgba{
									_toolkit.get_Theme()
										.Inactive
								};
								if (state) {
									targetVertex[0] = AnimationLength;
									targetRgba = _toolkit.get_Theme()
										.Active;
								}
								control.set_AnimationSize(targetVertex);
								auto animationVertex = control.get_AnimationSize()
									- control.get_AnimationPoint();
								control.set_AnimationContext(
									typename TPlug::context_t{
										_toolkit.get_Palette()
											.Interpolate(targetRgba, control.get_FrameColor()),
										sum(animationVertex * animationVertex)
									});
							};

						private:
							TToolkit &_toolkit;
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

							inline void operator()(TPlug &control) const {
								auto animationVertex = control.get_AnimationSize()
									- control.get_AnimationPoint();
								num_t remaining2{
									sum(animationVertex * animationVertex)
								};
								typename TPlug::context_t const &context{
									control.get_AnimationContext()
								};
								num_t rI{
									sqrt(push(context.distance2) / remaining2)
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
