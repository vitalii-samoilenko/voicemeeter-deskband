#pragma once

#include "Voicemeeter.UI.Cherry/Policies/Glyph/Updates/Knob.h"

#include "../../../Graphics/Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						template<
							typename TBundle,
							typename TGlyph>
						using Knob = Cherry::Policies::Glyph::Updates::Knob<
							Graphics::Instrumentation,
							TBundle,
							TGlyph>;
					}
				}
			}
		}
	}
}