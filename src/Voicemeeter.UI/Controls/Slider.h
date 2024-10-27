#pragma once

#include "../Graphics/Glyphs/Frame.h"

#include "State.h"

namespace Voicemeeter {
	namespace UI {
		namespace Controls {
			template<typename TGlyph>
			using Slider = State<int, TGlyph>;
		}
	}
}