#pragma once

#include "Voicemeeter.UI/Policies/CheckboxInteractivity.h"

#include "../Graphics/Glyphs/Out.h"

using namespace Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				using OutInteractivity = CheckboxInteractivity<Graphics::Glyphs::Out>;
			}
		}
	}
}