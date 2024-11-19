#pragma once

#include "../../../Graphics/Glyphs/Vban.h"
#include "Passthrough.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						using Vban = Passthrough<Graphics::Glyphs::Vban, int>;
					}
				}
			}
		}
	}
}