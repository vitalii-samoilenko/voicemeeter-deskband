#pragma once

#include "Voicemeeter.UI/Controls/Knob.h"

#include "../Policies/Glyph/Updates/Knob.h"
#include "../Policies/State/Changes/Knob.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Controls {
				template<
					typename TBundle,
					typename TGlyph,
					typename TChangeNotify>
				using Knob = UI::Controls::Knob<
					Policies::State::Changes::Knob::DEFAULT, Policies::State::Changes::Knob::MIN, Policies::State::Changes::Knob::MAX, Policies::State::Changes::Knob::DELTA,
					TGlyph,
					Policies::State::Changes::Knob::Set,
					TChangeNotify,
					Policies::Glyph::Updates::Knob<TBundle, TGlyph>>;
			}
		}
	}
}