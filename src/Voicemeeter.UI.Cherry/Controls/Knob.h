#pragma once

#include "Voicemeeter.UI/Controls/State.h"
#include "Voicemeeter.UI/Policies/Size/Scales/PreserveRatio.h"

#include "../Policies/State/Changes/Knob.h"
#include "../States/Knob.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Controls {
				template<
					typename TGlyph,
					typename TChangeNotify,
					typename TGlyphUpdate>
				using Knob = UI::Controls::State<
					States::Knob,
					TGlyph,
					Policies::State::Changes::Knob::Default,
					Policies::State::Changes::Knob::Next,
					Policies::State::Changes::Knob::Previous,
					Policies::State::Changes::Knob::Set,
					TChangeNotify,
					TGlyphUpdate,
					Policies::Size::Scales::PreserveRatio>;
			}
		}
	}
}