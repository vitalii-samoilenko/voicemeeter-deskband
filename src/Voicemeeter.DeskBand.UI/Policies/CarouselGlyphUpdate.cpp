#include "CarouselGlyphUpdate.h"

using namespace ::Voicemeeter::DeskBand::UI::Policies;

void CarouselGlyphUpdate::Update(Glyphs::Frame& glyph, const int& state) const {
	glyph.set_Frame(state);
}