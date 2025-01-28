#pragma once

#include "Voicemeeter.UI.Cherry/Policies/Glyph/Updates/Animations/Plug.h"

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
							using Plug = Cherry::Policies::Glyph::Updates::Animations::Plug<
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