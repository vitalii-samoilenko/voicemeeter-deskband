#pragma once

#include "../Policies/Size/Scales/PreserveRatio.h"
#include "../Policies/State/Changes/Checkbox.h"
#include "State.h"

namespace Voicemeeter {
	namespace UI {
		namespace Controls {
			template<
				typename TGlyph,
				typename TChangeNotify,
				typename TGlyphUpdate>
			using Checkbox = State<
				int,
				TGlyph,
				Policies::State::Changes::Checkbox::Default,
				Policies::State::Changes::Checkbox::Next,
				Policies::State::Changes::Checkbox::Previous,
				Policies::State::Changes::Checkbox::Set,
				TChangeNotify,
				TGlyphUpdate,
				Policies::Size::Scales::PreserveRatio>;
		}
	}
}