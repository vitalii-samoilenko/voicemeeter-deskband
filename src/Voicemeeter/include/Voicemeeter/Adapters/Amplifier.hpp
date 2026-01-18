#ifndef VOICEMEETER_ADAPTERS_AMPLIFIER_HPP
#define VOICEMEETER_ADAPTERS_AMPLIFIER_HPP

#include <utility>

#include "wheel.hpp"

namespace Voicemeeter {
	namespace Adapters {
		template<typename TBus>
		class Amplifier : public TBus {
		public:
			template<typename... Args>
			inline explicit Amplifier(Args &&...args)
				: TBus{ ::std::forward<Args>(args)... }
				, _gain{ 0 } {

			};
			Amplifier(Amplifier const &) = delete;
			Amplifier(Amplifier &&) = delete;

			inline ~Amplifier() = default;

			Amplifier & operator=(Amplifier const &) = delete;
			Amplifier & operator=(Amplifier &&) = delete;

			inline num_t get_Gain() const {
				return _gain;
			};
			inline void set_Gain(num_t value) {
				_gain = value;
			};

		private:
			num_t _gain;
		};
	}
}

#endif
