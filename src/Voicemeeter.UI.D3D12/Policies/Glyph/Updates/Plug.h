#pragma once

#include "Voicemeeter.UI.Cherry/Policies/Glyph/Updates/Plug.h"

#include "../../../Graphics/Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace D3D12 {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						template<typename TBundle, typename TGlyph>
						using Plug = Cherry::Policies::Glyph::Updates::Plug<
							Graphics::Instrumentation,
							TBundle,
							TGlyph>;
					}
				}
			}
		}
	}
}