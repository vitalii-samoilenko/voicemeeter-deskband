#pragma once

#include <type_traits>

#include "../Graphics/IGlyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			template<typename TGlyph, typename TState>
			class IGlyphUpdate {
				static_assert(
					::std::is_base_of_v<Graphics::IGlyph, TGlyph>,
					"TGlyph must be derived from IGlyph");

			public:
				IGlyphUpdate(const IGlyphUpdate&) = delete;
				IGlyphUpdate(IGlyphUpdate&&) = delete;

				virtual ~IGlyphUpdate() = default;

				IGlyphUpdate& operator=(const IGlyphUpdate&) = delete;
				IGlyphUpdate& operator=(IGlyphUpdate&&) = delete;

				virtual void Update(TGlyph& glyph, const TState& state) const = 0;

			protected:
				IGlyphUpdate() = default;
			};
		}
	}
}