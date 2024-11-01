#include "KnobGlyphUpdate.h"

using namespace ::Voicemeeter::UI::D2D::Policies;

void KnobGlyphUpdate::Update(Graphics::Glyphs::Knob& glyph, const States::Knob& state) const {
	glyph.set_Gain(state.gain / 100.F);
	glyph.set_LeftLevel(state.leftLevel / 100.F);
	glyph.set_RightLevel(state.rightLevel / 100.F);
	glyph.set_Enabled(state.enabled);
	glyph.set_Pinned(state.pinned);
}