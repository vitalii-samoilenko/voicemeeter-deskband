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
					, _slot{}
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
					return _slot.value();
				};
				inline void set_State(state_t const &value) {
					_slot = value;
					_notify(_slot.value());
					_update(*this, _slot.value());
				};

			private:
				::std::optional<state_t> _slot;
				TNotify _notify;
				TUpdate _update;
			};
		}
	}
}

#endif
