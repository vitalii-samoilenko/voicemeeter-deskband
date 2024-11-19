#pragma once

#include "../../../Graphics/Glyphs/Plug.h"
#include "Passthrough.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						using Plug = Passthrough<Graphics::Glyphs::Plug, int>;
					}
				}
			}
		}
	}
}