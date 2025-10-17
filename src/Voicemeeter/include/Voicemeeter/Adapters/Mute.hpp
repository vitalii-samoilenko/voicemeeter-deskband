#ifndef VOICEMEETER_ADAPTERS_MUTE_HPP
#define VOICEMEETER_ADAPTERS_MUTE_HPP

#include <utility>

namespace Voicemeeter {
	namespace Adapters {
		template<typename TBus>
		class Mute : public TBus {
		public:
			template<typename... Args>
			inline explicit Mute(Args &&...args)
				: TBus{ ::std::forward<Args>(args)... }
				, _mute{ false } {

			};
			Mute(Mute const &) = delete;
			Mute(Mute &&) = delete;

			inline ~Mute() = default;

			Mute & operator=(Mute const &) = delete;
			Mute & operator=(Mute &&) = delete;

			inline bool get_Mute() const {
				return _mute;
			};
			inline void set_Mute(bool value) {
				_mute = value;
			};

		private:
			bool _mute;
		};
	}
}

#endif
