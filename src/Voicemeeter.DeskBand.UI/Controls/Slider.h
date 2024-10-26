#pragma once

#include "State.h"
#include "../Graphics/Glyphs/Frame.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Controls {
				template<typename TGlyph>
				using Slider = State<int, TGlyph>;
			}
		}
	}
}