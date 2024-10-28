#pragma once

#include "IGlyphUpdate.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			template<typename TGlyph>
			using SliderGlyphUpdate = IGlyphUpdate<TGlyph, int>;
		}
	}
}