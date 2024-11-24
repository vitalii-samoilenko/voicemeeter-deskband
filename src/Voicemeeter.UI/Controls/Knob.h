#pragma once

#include "../Policies/Size/Scales/PreserveRatio.h"
#include "../Policies/State/Changes/Knob.h"
#include "../States/Knob.h"
#include "State.h"

namespace Voicemeeter {
	namespace UI {
		namespace Controls {
			template<
				int Default, int Min, int Max, int Delta,
				typename TGlyph,
				typename TStateSet,
				typename TChangeNotify,
				typename TGlyphUpdate>
			using Knob = State<
				States::Knob,
				TGlyph,
				Policies::State::Changes::Knob::Default<Default>,
				Policies::State::Changes::Knob::Next<Max, Delta>,
				Policies::State::Changes::Knob::Previous<Min, Delta>,
				TStateSet,
				TChangeNotify,
				TGlyphUpdate,
				Policies::Size::Scales::PreserveRatio>;
		}
	}
}