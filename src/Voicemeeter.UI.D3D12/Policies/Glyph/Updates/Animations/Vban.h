#pragma once

#include "Voicemeeter.UI.Cherry/Policies/Glyph/Updates/Animations/Vban.h"

#include "../../../../Graphics/Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace D3D12 {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						namespace Animations {
							template<
								typename TBundle,
								typename TAnimation,
								typename TGlyph>
							using Vban = Cherry::Policies::Glyph::Updates::Animations::Vban<
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