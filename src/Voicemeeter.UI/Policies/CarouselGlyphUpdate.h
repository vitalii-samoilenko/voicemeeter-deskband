#pragma once

#include "../Graphics/Glyphs/Frame.h"

#include "IGlyphUpdate.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			class CarouselGlyphUpdate : public IGlyphUpdate<Graphics::Glyphs::Frame, int> {
			public:
				CarouselGlyphUpdate() = default;
				CarouselGlyphUpdate(const CarouselGlyphUpdate&) = delete;
				CarouselGlyphUpdate(CarouselGlyphUpdate&&) = delete;

				~CarouselGlyphUpdate() = default;

				CarouselGlyphUpdate& operator=(const CarouselGlyphUpdate&) = delete;
				CarouselGlyphUpdate& operator=(CarouselGlyphUpdate&&) = delete;

				virtual void Update(Graphics::Glyphs::Frame& glyph, const int& state) const override;
			};
		}
	}
}