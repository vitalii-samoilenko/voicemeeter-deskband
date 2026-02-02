#ifndef VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_PLUG_HPP
#define VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_PLUG_HPP

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Control {
				namespace Updates {
					template<typename TToolkit>
					class Plug {
					public:
						using state_t = num_t;

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
						inline void operator()(TPlug &control, state_t state) const {
							control.set_FrameColor(
								state
									? _toolkit.get_Theme()
										.Active
									: _toolkit.get_Theme()
										.Inactive
							);
						};

					private:
						TToolkit &_toolkit;
					};
				}
			}
		}
	}
}

#endif
