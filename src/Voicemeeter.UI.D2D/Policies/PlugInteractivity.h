#pragma once

#include "Voicemeeter.UI/Policies/CheckboxInteractivity.h"

#include "../Graphics/Glyphs/Plug.h"

using namespace Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				using PlugInteractivity = CheckboxInteractivity<Graphics::Glyphs::Plug>;
			}
		}
	}
}