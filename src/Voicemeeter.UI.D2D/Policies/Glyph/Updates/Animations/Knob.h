#pragma once

#include "Voicemeeter.UI.Cherry/Policies/Glyph/Updates/Animations/Knob.h"

#include "../../../../Graphics/Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						namespace Animations {
							template<
								typename TBundle,
								typename TAnimation,
								typename TGlyph>
							using Knob = Cherry::Policies::Glyph::Updates::Animations::Knob<
								Graphics::Instrumentation,
								TBundle,
								TAnimation,
								TGlyph>;
						}
					}
				}
			}
		}
	}
}