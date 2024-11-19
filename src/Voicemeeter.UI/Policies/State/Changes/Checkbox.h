#pragma once

#include "Circular.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace State {
				namespace Changes {
					using Checkbox = Circular<int, 0, 0, 1, 1>;
				}
			}
		}
	}
}