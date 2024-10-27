#include "CarouselGlyphUpdate.h"

using namespace ::Voicemeeter::UI::Policies;

void CarouselGlyphUpdate::Update(Graphics::Glyphs::Frame& glyph, const int& state) const {
	glyph.set_Frame(state);
}