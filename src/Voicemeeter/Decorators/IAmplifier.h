#pragma once

#include <type_traits>

#include "../IBus.h"

namespace Voicemeeter {
	namespace Decorators {
		template<typename TBus>
		class IAmplifier : public TBus {
			static_assert(
				::std::is_base_of_v<IBus, TBus>,
				"TBus must be derived from IBus");

		public:
			IAmplifier(const IAmplifier&) = delete;
			IAmplifier(IAmplifier&&) = delete;

			IAmplifier& operator=(const IAmplifier&) = delete;
			IAmplifier& operator=(IAmplifier&&) = delete;

			virtual double get_Gain() const = 0;
			virtual void set_Gain(double value) = 0;
			virtual bool get_Mute() const = 0;
			virtual void set_Mute(bool value) = 0;

		private:
			IAmplifier() = default;

			~IAmplifier() = default;
		};
	}
}