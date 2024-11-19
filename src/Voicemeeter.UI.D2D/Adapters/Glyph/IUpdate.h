#pragma once

#include <type_traits>
#include <utility>

#include "../../Graphics/Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					template<typename TGlyph, typename TState>
					class IUpdate : public TGlyph {
						static_assert(
							::std::is_base_of_v<Graphics::Glyph, TGlyph>,
							"TGlyph must be derived from Glyph");

					public:
						IUpdate() = delete;
						IUpdate(const IUpdate&) = delete;
						IUpdate(IUpdate&&) = delete;

						IUpdate& operator=(const IUpdate&) = delete;
						IUpdate& operator=(IUpdate&&) = delete;

						virtual void Update(const TState& state) = 0;

					protected:
						template<typename... Args>
						explicit IUpdate(
							Args&&... args
						) : TGlyph{ ::std::forward<Args>(args)... } {

						}

						~IUpdate() = default;
					};
				}
			}
		}
	}
}