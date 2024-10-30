#pragma once

#include "Voicemeeter.UI/Policies/IGlyphUpdate.h"

#include "../Graphics/Glyphs/Vban.h"

using namespace ::Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				class VbanGlyphUpdate : public IGlyphUpdate<Graphics::Glyphs::Vban, int> {
				public:
					VbanGlyphUpdate() = default;
					VbanGlyphUpdate(const VbanGlyphUpdate&) = delete;
					VbanGlyphUpdate(VbanGlyphUpdate&&) = delete;

					~VbanGlyphUpdate() = default;

					VbanGlyphUpdate& operator=(const VbanGlyphUpdate&) = delete;
					VbanGlyphUpdate& operator=(VbanGlyphUpdate&&) = delete;

					virtual void Update(Graphics::Glyphs::Vban& glyph, const int& state) const override;
				};
			}
		}
	}
}