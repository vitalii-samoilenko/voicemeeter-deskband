#pragma once

#include "../Traits/IMovable.h"
#include "../Traits/IScalable.h"
#include "../Traits/IVisible.h"

namespace Voicemeeter {
		namespace UI {
			namespace Graphics {
				class IGlyph
					: public Traits::IVisible
					, public Traits::IMovable
					, public Traits::IScalable {
				public:
					IGlyph(const IGlyph&) = delete;
					IGlyph(IGlyph&&) = delete;

					virtual ~IGlyph() = default;

					IGlyph& operator=(const IGlyph&) = delete;
					IGlyph& operator=(IGlyph&&) = delete;

				protected:
					IGlyph() = default;
				};
			}
		}
}