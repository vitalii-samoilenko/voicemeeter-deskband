#pragma once

#include "Voicemeeter.UI/Policies/IGlyphUpdate.h"
#include "Voicemeeter.UI/States/Knob.h"

#include "../Graphics/Glyphs/Knob.h"

using namespace ::Voicemeeter::UI::Policies;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				class KnobGlyphUpdate : public IGlyphUpdate<Graphics::Glyphs::Knob, States::Knob> {
				public:
					KnobGlyphUpdate() = default;
					KnobGlyphUpdate(const KnobGlyphUpdate&) = delete;
					KnobGlyphUpdate(KnobGlyphUpdate&&) = delete;

					~KnobGlyphUpdate() = default;

					KnobGlyphUpdate& operator=(const KnobGlyphUpdate&) = delete;
					KnobGlyphUpdate& operator=(KnobGlyphUpdate&&) = delete;

					virtual void Update(Graphics::Glyphs::Knob& glyph, const States::Knob& state) const override;
				};
			}
		}
	}
}