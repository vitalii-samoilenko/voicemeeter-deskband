#pragma once

#include "Voicemeeter.UI.Cherry/Policies/Glyph/Updates/Vban.h"

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
						using Vban = Cherry::Policies::Glyph::Updates::Vban<
							Graphics::Instrumentation,
							TBundle,
							TGlyph>;
					}
				}
			}
		}
	}
}