#pragma once

#include <type_traits>

#include "estd/type_traits.h"

#include "../Graphics/IGlyph.h"

using namespace ::Voicemeeter::DeskBand::UI::Graphics;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TGlyph, typename TState>
				class IGlyphUpdate {
					static_assert(
						::std::is_base_of_v<IGlyph, ::estd::remove_cvref_t<TGlyph>>,
						"TGlyph must be derived from IGlyph");

				public:
					IGlyphUpdate(const IGlyphUpdate&) = delete;
					IGlyphUpdate(IGlyphUpdate&&) = delete;

					virtual ~IGlyphUpdate() = default;

					IGlyphUpdate& operator=(const IGlyphUpdate&) = delete;
					IGlyphUpdate& operator=(IGlyphUpdate&&) = delete;

					virtual void Update(::estd::remove_cvref_t<TGlyph>& glyph, const ::estd::remove_cvref_t<TState>& state) const = 0;

				protected:
					IGlyphUpdate() = default;
				};
			}
		}
	}
}