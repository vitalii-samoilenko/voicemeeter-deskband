#include "VbanGlyphUpdate.h"

using namespace ::Voicemeeter::UI::D2D::Policies;

void VbanGlyphUpdate::Update(Graphics::Glyphs::Vban& glyph, const int& state) const {
	glyph.set_Active(static_cast<bool>(state));
}