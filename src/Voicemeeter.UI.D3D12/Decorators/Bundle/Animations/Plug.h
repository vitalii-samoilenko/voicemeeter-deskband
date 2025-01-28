#pragma once

#include "Voicemeeter.UI.Cherry/Decorators/Bundle/Animations/Plug.h"

#include "../../../Graphics/Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace D3D12 {
			namespace Decorators {
				namespace Bundle {
					namespace Animations {
						template<typename TBundle>
						using Plug = Cherry::Decorators::Bundle::Animations::Plug<
							Graphics::Instrumentation,
							TBundle>;
					}
				}
			}
		}
	}
}