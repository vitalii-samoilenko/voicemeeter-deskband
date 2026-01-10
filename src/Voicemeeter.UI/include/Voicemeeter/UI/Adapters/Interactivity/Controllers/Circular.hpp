#ifndef VOICEMEETER_UI_ADAPTERS_INTERACTIVITY_CONTROLLERS_CIRCULAR_HPP
#define VOICEMEETER_UI_ADAPTERS_INTERACTIVITY_CONTROLLERS_CIRCULAR_HPP

#include <utility>

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			namespace Interactivity {
				namespace Controllers {
					template<typename TStateControl>
					class Circular : public TStateControl {
					public:
						template<typename... Args>
						inline Circular(
							state_t const &min,
							state_t const &max,
							state_t const &delta,
							Args &&...args)
							: TStateControl{ ::std::forward<Args>(args)... }
							, _min{ min }
							, _max{ max }
							, _delta{ delta } {

						};
						Circular(Circular const &) = delete;
						Circular(Circular &&) = delete;

						inline ~Circular() = default;

						Circular & operator=(Circular const &) = delete;
						Circular & operator=(Circular &&) = delete;

						inline void set_NextState() {
							state_t dst{ TStateControl::get_State() };
							if (_max - _delta < dst) {
								dst = _min;
							} else {
								dst += _delta;
							}
							TStateControl::set_State(dst);
						};

					private:
						using state_t = decltype(TStateControl::get_State());

						state_t _min;
						state_t _max;
						state_t _delta;
					};
				}
			}
		}
	}
}

#endif
