#pragma once

#include <type_traits>

#include "../Mixer.h"

namespace Voicemeeter {
	namespace Adapters {
		template<typename Specification,
			typename TPILine, typename TPIStrip, typename TPInput,
			typename TVILine, typename TVIStrip, typename TVInput,
			typename TPOLine, typename TPOStrip, typename TPOutput,
			typename TVOLine, typename TVOStrip, typename TVOutput,
			typename TMixer>
		class Network : public TMixer {
			static_assert(
				::std::is_base_of_v<Mixer<Specification,
					TPILine, TPIStrip, TPInput,
					TVILine, TVIStrip, TVInput,
					TPOLine, TPOStrip, TPOutput,
					TVOLine, TVOStrip, TVOutput>, TMixer>,
				"TMixer must be derived from Mixer");

		public:
			using TMixer::TMixer;

			Network() = delete;
			Network(const Network&) = delete;
			Network(Network&&) = delete;

			inline ~Network() = default;

			Network& operator=(const Network&) = delete;
			Network& operator=(Network&&) = delete;

			inline bool get_Vban() const {
				return m_vban;
			};
			inline void set_Vban(bool value) {
				m_vban = value;
			};

		private:
			bool m_vban;
		};
	}
}