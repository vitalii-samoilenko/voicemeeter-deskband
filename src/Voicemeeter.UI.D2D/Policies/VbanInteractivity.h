#pragma once

#include "Voicemeeter.UI/Policies/CheckboxInteractivity.h"

#include "../Graphics/Glyphs/Vban.h"

using namespace Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				using VbanInteractivity = CheckboxInteractivity<Graphics::Glyphs::Vban>;
			}
		}
	}
}