#pragma once

#include "Voicemeeter.UI.Cherry/Decorators/Bundle/Animations/Vban.h"

#include "../../../Graphics/Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace D3D12 {
			namespace Decorators {
				namespace Bundle {
					namespace Animations {
						template<typename TBundle>
						using Vban = Cherry::Decorators::Bundle::Animations::Vban<
							Graphics::Instrumentation,
							TBundle>;
					}
				}
			}
		}
	}
}