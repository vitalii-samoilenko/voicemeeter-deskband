#pragma once

#include "../Graphics/Glyphs/Frame.h"
#include "IGlyphUpdate.h"

using namespace ::Voicemeeter::DeskBand::UI::Graphics;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				class CarouselGlyphUpdate final : public IGlyphUpdate<Glyphs::Frame, int> {
				public:
					CarouselGlyphUpdate() = default;
					CarouselGlyphUpdate(const CarouselGlyphUpdate&) = delete;
					CarouselGlyphUpdate(CarouselGlyphUpdate&&) = delete;

					~CarouselGlyphUpdate() = default;

					CarouselGlyphUpdate& operator=(const CarouselGlyphUpdate&) = delete;
					CarouselGlyphUpdate& operator=(CarouselGlyphUpdate&&) = delete;

					virtual void Update(Glyphs::Frame& glyph, const int& state) const override;
				};
			}
		}
	}
}