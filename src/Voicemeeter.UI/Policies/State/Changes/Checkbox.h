#pragma once

#include "Basic.h"
#include "Circular.h"
#include "Range.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace State {
				namespace Changes {
					namespace Checkbox {
						using Default = Basic::Default<int, 0>;
						using Next = Circular::Next<int, 0, 1, 1>;
						using Previous = Circular::Previous<int, 0, 1, 1>;
						using Set = Range::Set<int, 0, 1>;
					}
				}
			}
		}
	}
}