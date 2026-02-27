#ifndef VOICEMEETER_UI_ADAPTERS_STATE_HPP
#define VOICEMEETER_UI_ADAPTERS_STATE_HPP

#include <optional>
#include <utility>

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			template<
				typename TControl,
				typename TValue,
				typename TNotify,
				typename TUpdate>
			class State : public TControl {
			public:
				using state_t = TValue;

				template<typename ...Args>
				inline State(
					TNotify &&notify = TNotify{},
					TUpdate &&update = TUpdate{},
					Args &&...args)
					: TControl{ ::std::forward<Args>(args) ... }
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

				inline state_t const & get_State() const {
					return _state.value();
				};
				inline void set_State(state_t const &value) {
					_state = value;
					_notify(value);
					_update(this, value);
				};
				inline void owr_State(state_t const &value) {
					_state = value;
					_update(this, value);
				};

			private:
				::std::optional<state_t> _state;
				TNotify _notify;
				TUpdate _update;
			};
		}
	}
}

#endif
