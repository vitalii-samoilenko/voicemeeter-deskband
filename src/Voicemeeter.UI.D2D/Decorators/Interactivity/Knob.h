#pragma once

#include "Voicemeeter.UI/Decorators/Interactivity/Knob.h"

#include "../../Controls/Knob.h"
#include "../../Graphics/Glyphs/Knob.h"
#include "../../Policies/Glyph/Updates/Knob.h"
#include "../../Policies/State/Changes/Knob.h"

using namespace Voicemeeter::UI::Decorators::Interactivity;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				namespace Interactivity {
					template<
						UI::Direction Direction,
						typename TGlyph,
						typename TChangeNotify>
					using Knob = UI::Decorators::Interactivity::Knob<
						Direction,
						Controls::Knob<
							TGlyph,
							TChangeNotify>,
						Policies::State::Changes::Knob::DEFAULT, Policies::State::Changes::Knob::MIN, Policies::State::Changes::Knob::MAX, Policies::State::Changes::Knob::DELTA,
						Policies::State::Changes::Knob::Set,
						TGlyph,
						TChangeNotify,
						Policies::Glyph::Updates::Knob<TGlyph>>;
				}
			}
		}
	}
}