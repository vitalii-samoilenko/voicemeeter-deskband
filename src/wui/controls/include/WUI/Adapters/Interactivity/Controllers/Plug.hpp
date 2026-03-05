#ifndef WUI_ADAPTERS_INTERACTIVITY_CONTROLLERS_PLUG_HPP
#define WUI_ADAPTERS_INTERACTIVITY_CONTROLLERS_PLUG_HPP

#include <utility>

namespace WUI {
	namespace Adapters {
		namespace Interactivity {
			namespace Controllers {
				template<typename TPlug>
				class Plug : public TPlug {
				public:
					using state_t = typename TPlug::state_t;

					template<typename ...Args>
					inline Plug(Args &&...args)
						: TPlug{ ::std::forward<Args>(args) ... } {

					};
					Plug(Plug const &) = delete;
					Plug(Plug &&) = delete;

					inline ~Plug() = default;

					Plug & operator=(Plug const &) = delete;
					Plug & operator=(Plug &&) = delete;

					inline void set_NextState() {
						state_t state{ TPlug::get_State() };
						state.toggle = !state.toggle;
						TPlug::set_State(state);
					};
				};
			}
		}
	}
}

#endif
