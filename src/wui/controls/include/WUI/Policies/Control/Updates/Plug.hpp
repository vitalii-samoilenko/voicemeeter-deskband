#ifndef WUI_POLICIES_CONTROL_UPDATES_PLUG_HPP
#define WUI_POLICIES_CONTROL_UPDATES_PLUG_HPP

#include "math.hpp"

#include "WUI/States/Plug.hpp"

namespace WUI {
	namespace Policies {
		namespace Control {
			namespace Updates {
				template<typename TToolkit>
				class Plug {
				public:
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
						vec_t const *targetRgba{
							state.toggle
								? &_toolkit.get_Theme()
									.Active
								: &_toolkit.get_Theme()
									.Inactive
						};
						that->set_FrameColor(*targetRgba);
						that->set_LabelColor(*targetRgba);
					};

				private:
					TToolkit &_toolkit;
				};
			}
		}
	}
}

#endif
