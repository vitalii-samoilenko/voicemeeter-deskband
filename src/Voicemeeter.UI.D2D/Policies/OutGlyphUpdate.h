#pragma once

#include "Voicemeeter.UI/Policies/IGlyphUpdate.h"

#include "../Graphics/Glyphs/Out.h"

using namespace ::Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				class OutGlyphUpdate : public IGlyphUpdate<Graphics::Glyphs::Out, int> {
				public:
					OutGlyphUpdate() = default;
					OutGlyphUpdate(const OutGlyphUpdate&) = delete;
					OutGlyphUpdate(OutGlyphUpdate&&) = delete;

					~OutGlyphUpdate() = default;

					OutGlyphUpdate& operator=(const OutGlyphUpdate&) = delete;
					OutGlyphUpdate& operator=(OutGlyphUpdate&&) = delete;

					virtual void Update(Graphics::Glyphs::Out& glyph, const int& state) const override;
				};
			}
		}
	}
}