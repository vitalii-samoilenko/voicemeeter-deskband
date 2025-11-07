#ifndef VOICEMEETER_ADAPTERS_AMPLIFIER_HPP
#define VOICEMEETER_ADAPTERS_AMPLIFIER_HPP

#include <utility>

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

			inline int get_Gain() const {
				return _gain;
			};
			inline void set_Gain(int value) {
				_gain = value;
			};

		private:
			int _gain;
		};
	}
}

#endif
