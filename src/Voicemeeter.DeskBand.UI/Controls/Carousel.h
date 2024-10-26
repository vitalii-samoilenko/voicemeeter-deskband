#pragma once

#include "../Graphics/Glyphs/Frame.h"

#include "State.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Controls {
				using Carousel = State<int, Glyphs::Frame>;
			}
		}
	}
}