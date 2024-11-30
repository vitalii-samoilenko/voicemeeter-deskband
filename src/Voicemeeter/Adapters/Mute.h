#pragma once

#include <concepts>
#include <type_traits>

#include "../Channel.h"

namespace Voicemeeter {
	namespace Adapters {
		template<typename Specification, typename TLine,
			typename TChannel>
		class Mute : public TChannel {
			static_assert(
				::std::is_base_of_v<Channel<Specification, TLine>, TChannel>,
				"TChannel must be derived from Channel");
			static_assert(
				::std::is_move_constructible_v<TChannel>,
				"TChannel must be move constructible");

		public:
			using TChannel::TChannel;

			Mute() = delete;
			Mute(const Mute&) = delete;
			inline Mute(Mute&&) = default;

			inline ~Mute() = default;

			Mute& operator=(const Mute&) = delete;
			Mute& operator=(Mute&&) = delete;

			inline bool get_Mute() const {
				return m_mute;
			};
			inline void set_Mute(bool value) {
				m_mute = value;
			};

		private:
			bool m_mute;
		};
	}
}