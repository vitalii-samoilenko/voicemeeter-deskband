#pragma once

#include "../States/Knob.h"

#include "State.h"

namespace Voicemeeter {
	namespace UI {
		namespace Controls {
			template<typename TGlyph>
			using Knob = State<States::Knob, TGlyph>;
		}
	}
}