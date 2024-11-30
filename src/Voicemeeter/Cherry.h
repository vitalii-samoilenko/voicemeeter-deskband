#pragma once

#include "Adapters/Network.h"

namespace Voicemeeter {
	struct CherrySpecification {
		struct Input {
			struct Physical {
				struct Strip {
					static constexpr size_t Width{ 2 };
				};
				static constexpr size_t Width{ 1 };
			};
			struct Virtual {
				struct Strip {
					static constexpr size_t Width{ 8 };
				};
				static constexpr size_t Width{ 1 };
			};
		};
		struct Output {
			struct Physical {
				struct Strip {
					static constexpr size_t Width{ 8 };
				};
				static constexpr size_t Width{ 2 };
			};
			struct Virtual {
				struct Strip {
					static constexpr size_t Width{ 8 };
				};
				static constexpr size_t Width{ 2 };
			};
		};
	};
}