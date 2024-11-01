#include "PlugGlyphUpdate.h"

using namespace ::Voicemeeter::UI::D2D::Policies;

void PlugGlyphUpdate::Update(Graphics::Glyphs::Plug& glyph, const int& state) const {
	glyph.set_Active(static_cast<bool>(state));
}