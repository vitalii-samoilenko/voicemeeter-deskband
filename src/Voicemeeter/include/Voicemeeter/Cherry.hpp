#ifndef VOICEMEETER_CHERRY_HPP
#define VOICEMEETER_CHERRY_HPP

#include "Voicemeeter/Adapters/Multiclient/Channel.hpp"
#include "Voicemeeter/Adapters/Multiclient/Mixer.hpp"
#include "Voicemeeter/Adapters/Multiclient/Network.hpp"
#include "Voicemeeter/Adapters/Multiclient/Strip.hpp"
#include "Voicemeeter/Adapters/Network.hpp"
#include "Voicemeeter/Adapters/Strip.hpp"
#include "Voicemeeter/Channel.hpp"
#include "Voicemeeter/Mixer.hpp"

namespace Voicemeeter {

	using PIStrip = Adapters::Multiclient::Strip<
		Adapters::Strip<
			2, Adapters::Multiclient::Channel<
				Channel>>>;
	using VIStrip = Adapters::Multiclient::Strip<
		Adapters::Strip<
			8, Adapters::Multiclient::Channel<
				Channel>>>;
	using POStrip = Adapters::Multiclient::Strip<
		Adapters::Strip<
			8, Adapters::Multiclient::Channel<
				Channel>>>;
	using VOStrip = Adapters::Multiclient::Strip<
		Adapters::Strip<
			8, Adapters::Multiclient::Channel<
				Channel>>>;

	using Cherry = Adapters::Multiclient::Network<
		Adapters::Multiclient::Mixer<
			1, PIStrip,
			1, VIStrip,
			2, POStrip,
			2, VOStrip,
			Adapters::Network<
				Mixer<
					1, PIStrip,
					1, VIStrip,
					2, POStrip,
					2, VOStrip
			>>
	>>;

}

#endif
