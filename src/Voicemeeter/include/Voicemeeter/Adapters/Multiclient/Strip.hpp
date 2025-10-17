#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_STRIP_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_STRIP_HPP

#include "Voicemeeter/Adapters/Multiclient/Amplifier.hpp"
#include "Voicemeeter/Adapters/Multiclient/Mute.hpp"

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {

			template<typename TStrip>
			using Strip = Amplifier<
				Mute<
					TStrip>>;

		}
	}
}

#endif
