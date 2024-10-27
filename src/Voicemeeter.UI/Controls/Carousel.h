#pragma once

#include "../Graphics/Glyphs/Frame.h"

#include "State.h"

namespace Voicemeeter {
	namespace UI {
		namespace Controls {
			using Carousel = State<int, Graphics::Glyphs::Frame>;
		}
	}
}