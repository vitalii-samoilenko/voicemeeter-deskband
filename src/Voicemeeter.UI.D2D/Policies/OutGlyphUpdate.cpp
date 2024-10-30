#include "OutGlyphUpdate.h"

using namespace ::Voicemeeter::UI::D2D::Policies;

void OutGlyphUpdate::Update(Graphics::Glyphs::Out& glyph, const int& state) const {
	glyph.set_Active(static_cast<bool>(state));
}