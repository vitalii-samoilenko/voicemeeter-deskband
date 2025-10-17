#ifndef VOICEMEETER_ADAPTERS_STRIP_HPP
#define VOICEMEETER_ADAPTERS_STRIP_HPP

#include "Voicemeeter/Adapters/Amplifier.hpp"
#include "Voicemeeter/Adapters/Mute.hpp"
#include "Voicemeeter/Bus.hpp"

namespace Voicemeeter {
	namespace Adapters {

		template<size_t Width, typename TChannel>
		using Strip = Amplifier<
			Mute<
				Bus<Width, TChannel>>

	}
}

#endif
