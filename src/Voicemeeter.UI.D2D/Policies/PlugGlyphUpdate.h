#pragma once

#include "Voicemeeter.UI/Policies/IGlyphUpdate.h"

#include "../Graphics/Glyphs/Plug.h"

using namespace ::Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				class PlugGlyphUpdate : public IGlyphUpdate<Graphics::Glyphs::Plug, int> {
				public:
					PlugGlyphUpdate() = default;
					PlugGlyphUpdate(const PlugGlyphUpdate&) = delete;
					PlugGlyphUpdate(PlugGlyphUpdate&&) = delete;

					~PlugGlyphUpdate() = default;

					PlugGlyphUpdate& operator=(const PlugGlyphUpdate&) = delete;
					PlugGlyphUpdate& operator=(PlugGlyphUpdate&&) = delete;

					virtual void Update(Graphics::Glyphs::Plug& glyph, const int& state) const override;
				};
			}
		}
	}
}