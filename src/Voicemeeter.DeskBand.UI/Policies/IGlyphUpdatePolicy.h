#pragma once

#include "estd/type_traits.h"

#include "../Graphics/IGlyph.h"

using namespace Voicemeeter::DeskBand::UI::Graphics;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TGlyph, typename TState>
				class IGlyphUpdatePolicy {
					static_assert(estd::is_base_of<IGlyph, TGlyph>(),
						"TGlyph must be derived from IGlyph");

				public:
					IGlyphUpdatePolicy() = delete;
					IGlyphUpdatePolicy(const IGlyphUpdatePolicy&) = delete;
					IGlyphUpdatePolicy(IGlyphUpdatePolicy&&) = delete;

					virtual ~IGlyphUpdatePolicy() = 0;

					IGlyphUpdatePolicy& operator=(const IGlyphUpdatePolicy&) = delete;
					IGlyphUpdatePolicy& operator=(IGlyphUpdatePolicy&&) = delete;

					const virtual void Update(estd::remove_cvref_t<TGlyph>& glyph, const estd::remove_cvref_t<TState>& state) = 0;
				};
			}
		}
	}
}