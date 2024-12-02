#pragma once

#include "Voicemeeter/Adapters/Multiclient/Cherry.h"

#include "Client.h"

namespace Voicemeeter {
	namespace Clients {
		namespace Remote {
			using Cherry = Client<Adapters::Multiclient::Cherry>;
		}
	}
}