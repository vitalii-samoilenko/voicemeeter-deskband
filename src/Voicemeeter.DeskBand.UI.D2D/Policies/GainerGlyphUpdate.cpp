#include "GainerGlyphUpdate.h"

using namespace ::Voicemeeter::DeskBand::UI::D2D::Policies;

void GainerGlyphUpdate::Update(Graphics::Glyphs::Gainer& glyph, const int& state) const {
	glyph.set_Level(state / 100.F);
}