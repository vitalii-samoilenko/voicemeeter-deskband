#pragma once

#include <type_traits>

#include "../../Graphics/IGlyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Glyph {
				template<typename TGlyph, typename TState>
				class IUpdate {
					static_assert(
						::std::is_base_of_v<Graphics::IGlyph, TGlyph>,
						"TGlyph must be derived from IGlyph");

				public:
					IUpdate(const IUpdate&) = delete;
					IUpdate(IUpdate&&) = delete;

					virtual ~IUpdate() = default;

					IUpdate& operator=(const IUpdate&) = delete;
					IUpdate& operator=(IUpdate&&) = delete;

					virtual void Update(TGlyph& glyph, const TState& state) const = 0;

				protected:
					IUpdate() = default;
				};
			}
		}
	}
}