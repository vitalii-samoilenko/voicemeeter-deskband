#ifndef VOICEMEETER_UI_ADAPTERS_STATE_HPP
#define VOICEMEETER_UI_ADAPTERS_STATE_HPP

#include <utility>

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			template<
				typename TControl,
				typename TState,
				typename TNotify,
				typename TUpdate>
			class State : public TControl {
			public:
				template<typename... Args>
				inline State(
					TNotify &&notify = TNotify{},
					TUpdate &&update = TUpdate{},
					Args &&...args)
					: TControl{ ::std::forward<Args>(args)... }
					, _state{}
					, _update{ ::std::move(update) }
					, _notify{ ::std::move(notify) } {

				};
				State() = delete;
				State(State const &) = delete;
				State(State &&) = delete;

				inline ~State() = default;

				State & operator=(State const &) = delete;
				State & operator=(State &&) = delete;

				inline TState const & get_State() const {
					return _state;
				};
				inline void set_State(TState const &value) {
					_state = value;
					_notify(_state);
					_update(*this, _state);
				};

			private:
				TState _state;
				TNotify _notify;
				TUpdate _update;
			};
		}
	}
}

#endif
