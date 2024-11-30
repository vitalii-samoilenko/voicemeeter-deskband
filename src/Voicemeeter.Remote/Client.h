#pragma once

#include "Environment/ITimer.h"
#include "Voicemeeter/Adapters/Multiclient/Cherry.h"

namespace Voicemeeter {
	namespace Remote {
		enum class Type : long {
			Voicemeeter = 1,
			Banana = 2,
			Potato = 3
		};

		class Client {
		public:
			Client(
				::Environment::ITimer& timer,
				::Voicemeeter::Adapters::Multiclient::Cherry& mixer
			);
			Client()
		};
	}
}