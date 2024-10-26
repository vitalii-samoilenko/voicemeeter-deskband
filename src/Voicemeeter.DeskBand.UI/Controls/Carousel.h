#pragma once

#include "State.h"
#include "../Graphics/Glyphs/Frame.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Controls {
				using Carousel = State<int, Glyphs::Frame>;
			}
		}
	}
}