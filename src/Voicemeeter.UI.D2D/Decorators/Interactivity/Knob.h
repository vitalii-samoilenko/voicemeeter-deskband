#pragma once

#include "Voicemeeter.UI/Decorators/Interactivity/Knob.h"

#include "../../Controls/Knob.h"
#include "../../Policies/Glyph/Updates/Knob.h"
#include "../../Policies/State/Changes/Knob.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				namespace Interactivity {
					template<
						UI::Direction Direction,
						typename TBundle,
						typename TGlyph,
						typename TChangeNotify>
					using Knob = UI::Decorators::Interactivity::Knob<
						Direction,
						Policies::State::Changes::Knob::DEFAULT, Policies::State::Changes::Knob::MIN, Policies::State::Changes::Knob::MAX, Policies::State::Changes::Knob::DELTA,
						TGlyph,
						Policies::State::Changes::Knob::Set,
						TChangeNotify,
						Policies::Glyph::Updates::Knob<TBundle, TGlyph>,
						Controls::Knob<
							TBundle,
							TGlyph,
							TChangeNotify>>;
				}
			}
		}
	}
}