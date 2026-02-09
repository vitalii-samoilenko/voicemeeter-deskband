#ifndef VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_PLUG_HPP
#define VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_ANIMATED_PLUG_HPP

#include "wheel.hpp"

#include "Voicemeeter/UI/States/Plug.hpp"

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

						template<typename TToolkit>
						class Plug {
						public:
							using context_t = PlugContext<TToolkit>;
							using state_t = States::Plug;

							inline explicit Plug(TToolkit &toolkit)
								: _toolkit{ toolkit } {

							};
							Plug() = delete;
							Plug(Plug const &) = delete;
							inline Plug(Plug &&) = default;

							inline ~Plug() = default;

							Plug & operator=(Plug const &) = delete;
							Plug & operator=(Plug &&) = delete;

							template<typename TPlug>
							inline void operator()(TPlug *that, state_t const &state) const {
								constexpr num_t AnimationLength{ 200 };
								vector_t targetVertex{ 0 };
								vector_t const *targetRgba{
									&_toolkit.get_Theme()
										.Inactive
								};
								if (state.toggle) {
									targetVertex[0] = AnimationLength;
									targetRgba = &_toolkit.get_Theme()
										.Active;
								}
								that->set_AnimationSize(targetVertex);
								auto animationVertex = that->get_AnimationSize()
									- that->get_AnimationPosition();
								that->set_AnimationContext(context_t{
									_toolkit.get_Palette()
										.Interpolate(*targetRgba, that->get_FrameColor()),
									sum(animationVertex * animationVertex)
								});
							};

						private:
							TToolkit &_toolkit;
						};

						template<typename TToolkit>
						class PlugFrame {
						public:
							using context_t = PlugContext<TToolkit>;

							inline explicit PlugFrame(TToolkit &toolkit)
								: _toolkit{ toolkit } {

							};
							PlugFrame() = delete;
							PlugFrame(PlugFrame const &) = delete;
							inline PlugFrame(PlugFrame &&) = default;

							inline ~PlugFrame() = default;

							PlugFrame & operator=(PlugFrame const &) = delete;
							PlugFrame & operator=(PlugFrame &&) = delete;

							template<typename TPlug>
							inline void operator()(TPlug *that) const {
								auto animationVertex = that->get_AnimationSize()
									- that->get_AnimationPosition();
								num_t remaining2{
									sum(animationVertex * animationVertex)
								};
								context_t const &context{
									that->get_AnimationContext()
								};
								num_t rI{
									remaining2
										? sqrt(push(context.distance2) / remaining2)
										: One
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
