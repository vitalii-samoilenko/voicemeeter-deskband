#pragma once

#include "Voicemeeter.UI/States/Knob.h"

#include "../../../Graphics/Glyphs/Knob.h"
#include "Passthrough.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						using Knob = Passthrough<Graphics::Glyphs::Knob, States::Knob>;
					}
				}
			}
		}
	}
}