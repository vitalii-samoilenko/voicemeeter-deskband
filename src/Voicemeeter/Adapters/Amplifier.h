#pragma once

#include <type_traits>

#include "../Channel.h"

namespace Voicemeeter {
	namespace Adapters {
		template<typename Specification, typename TLine,
			typename TChannel>
		class Amplifier : public TChannel {
			static_assert(
				::std::is_base_of_v<Channel<Specification, TLine>, TChannel>,
				"TChannel must be derived from Channel");
			//static_assert(
			//	::std::is_move_constructible_v<TChannel>,
			//	"TChannel must be move constructible");

		public:
			using TChannel::TChannel;

			Amplifier() = delete;
			Amplifier(const Amplifier&) = delete;
			inline Amplifier(Amplifier&&) = default;

			inline ~Amplifier() = default;

			Amplifier& operator=(const Amplifier&) = delete;
			Amplifier& operator=(Amplifier&&) = delete;

			inline double get_Gain() const {
				return m_gain;
			};
			inline void set_Gain(double value) {
				m_gain = value;
			};

		private:
			double m_gain;
		};
	}
}