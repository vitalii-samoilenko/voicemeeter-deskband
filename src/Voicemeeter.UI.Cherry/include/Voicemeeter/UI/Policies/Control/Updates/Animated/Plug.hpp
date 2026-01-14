#ifndef VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_PLUG_HPP
#define VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_PLUG_HPP

#include <cmath>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Control {
				namespace Updates {
					namespace Animated {
						template<typename TToolkit>
						struct PlugContext {
							typename TToolkit::Palette::gradient path;
							int distance2;
						};

						template<typename TToolkit, typename TPlug>
						struct Plug {
							inline void operator()(
								TPlug &control, typename TPlug::state_t state) const {
								control.set_AnimationSize(
									vector_t{ state * 200 });
								auto animationVertex = control.get_AnimationSize()
									- control.get_AnimationPoint();
								control.set_AnimationContext(
									typename TPlug::context_t{
										_toolkit.get_Palette()
											.Interpolate(
												state
													? _toolkit.get_Theme()
														.Active
													: _toolkit.get_Theme()
														.Inactive,
												control.get_FrameColor()),
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

							inline void operator()(TPlug &control) const {
								auto animationVertex = control.get_AnimationSize()
									- control.get_AnimationPoint();
								num_t remaining2{
									(animationVertex * animationVertex)
										.sum()
								};
								typename TPlug::context_t const &context{
									control.get_AnimationContext()
								};
								num_t rI{
									static_cast<num_t>(::std::sqrt(
										context.distance2 * SCALING_FACTOR / remaining2
										* SCALING_FACTOR))
								};
								glyph.set_FrameColor(
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
