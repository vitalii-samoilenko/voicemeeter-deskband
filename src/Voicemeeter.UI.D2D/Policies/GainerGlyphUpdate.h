#pragma once

#include "Voicemeeter.UI/Policies/SliderGlyphUpdate.h"

#include "../Graphics/Glyphs/Gainer.h"

using namespace ::Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				class GainerGlyphUpdate : public SliderGlyphUpdate<Graphics::Glyphs::Gainer> {
				public:
					GainerGlyphUpdate() = default;
					GainerGlyphUpdate(const GainerGlyphUpdate&) = delete;
					GainerGlyphUpdate(GainerGlyphUpdate&&) = delete;

					~GainerGlyphUpdate() = default;

					GainerGlyphUpdate& operator=(const GainerGlyphUpdate&) = delete;
					GainerGlyphUpdate& operator=(GainerGlyphUpdate&&) = delete;

					virtual void Update(Graphics::Glyphs::Gainer& glyph, const int& state) const override;
				};
			}
		}
	}
}