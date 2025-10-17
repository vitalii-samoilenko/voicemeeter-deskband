#ifndef VOICEMEETER_STRIP_HPP
#define VOICEMEETER_STRIP_HPP

#include "Voicemeeter/Adapters/Amplifier.hpp"
#include "Voicemeeter/Adapters/Mute.hpp"
#include "Voicemeeter/Bus.hpp"
#include "Voicemeeter/Channel.hpp"

namespace Voicemeeter {
	template<size_t Width>
	using Strip = Adapters::Amplifier<
		Adapters::Mute<
			Bus<Width, Channel>>>;
}

#endif
