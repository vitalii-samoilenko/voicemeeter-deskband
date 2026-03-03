#ifndef VOICEMEETER_CHERRY_HPP
#define VOICEMEETER_CHERRY_HPP

#include <stdexcept>
#include <tuple>
#include <utility>

#include "Voicemeeter/Adapters/Amplifier.hpp"
#include "Voicemeeter/Adapters/Mixer.hpp"
#include "Voicemeeter/Adapters/Multiclient/Amplifier.hpp"
#include "Voicemeeter/Adapters/Multiclient/Channel.hpp"
#include "Voicemeeter/Adapters/Multiclient/Mixer.hpp"
#include "Voicemeeter/Adapters/Multiclient/Mute.hpp"
#include "Voicemeeter/Adapters/Multiclient/Network.hpp"
#include "Voicemeeter/Adapters/Multiclient/Token.hpp"
#include "Voicemeeter/Adapters/Mute.hpp"
#include "Voicemeeter/Adapters/Network.hpp"
#include "Voicemeeter/Bus.hpp"
#include "Voicemeeter/Channel.hpp"

namespace Voicemeeter {
	namespace Layouts {
		class Cherry {
		public:
			template<typename ...TChannels>
			using Strip = Adapters::Multiclient::Mute<
				Adapters::Multiclient::Amplifier<
				Adapters::Multiclient::Token<
					Adapters::Mute<
					Adapters::Amplifier<
					Bus<
						TChannels ...
					>>>
				>>>;
			using Channel = Adapters::Multiclient::Channel<
				Adapters::Multiclient::Token<
					Channel
				>>;
			using PIStrip = Strip<
				Channel,
				Channel>;
			using VIStrip = Strip<
				Channel,
				Channel,
				Channel,
				Channel,
				Channel,
				Channel,
				Channel,
				Channel>;
			using POStrip = Strip<
				Channel,
				Channel,
				Channel,
				Channel,
				Channel,
				Channel,
				Channel,
				Channel>;
			using VOStrip = Strip<
				Channel,
				Channel,
				Channel,
				Channel,
				Channel,
				Channel,
				Channel,
				Channel>;

			enum Strips : size_t {
				P = 0,
				V = 1,
				A1 = 2,
				A2 = 3,
				B1 = 4,
				B2 = 5
			};

			static constexpr size_t InputSize{ V - P + 1 };
			static constexpr size_t OutputSize{ B2 - A1 + 1 };

			template<typename ...Args>
			inline explicit Cherry(Args &&...args)
				: _strips{ ::std::forward<Args>(args) ... } {

			};
			Cherry(Cherry const &) = delete;
			Cherry(Cherry &&) = delete;

			inline ~Cherry() = default;

			Cherry & operator=(Cherry const &) = delete;
			Cherry & operator=(Cherry &&) = delete;

			template<size_t I>
			inline auto & get_Strip() {
				return ::std::get<I>(_strips);
			};

		private:
			::std::tuple<
				PIStrip,
				VIStrip,
				POStrip,
				POStrip,
				VOStrip,
				VOStrip
			> _strips;
		};
	}

	using Cherry = Adapters::Multiclient::Network<
		Adapters::Multiclient::Mixer<
		Adapters::Multiclient::Token<
			Adapters::Network<
			Adapters::Mixer<
				Layouts::Cherry
			>>
		>>>;
}

#endif
