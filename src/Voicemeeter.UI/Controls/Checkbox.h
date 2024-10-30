#pragma once

#include "State.h"

namespace Voicemeeter {
	namespace UI {
		namespace Controls {
			template<typename TGlyph>
			using Checkbox = State<int, TGlyph>;
		}
	}
}