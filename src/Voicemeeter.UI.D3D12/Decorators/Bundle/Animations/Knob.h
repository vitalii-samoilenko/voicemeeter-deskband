#pragma once

#include "Voicemeeter.UI.Cherry/Decorators/Bundle/Animations/Knob.h"

#include "../../../Graphics/Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace D3D12 {
			namespace Decorators {
				namespace Bundle {
					namespace Animations {
						template<typename TBundle>
						using Knob = Cherry::Decorators::Bundle::Animations::Knob<
							Graphics::Instrumentation,
							TBundle>;
					}
				}
			}
		}
	}
}