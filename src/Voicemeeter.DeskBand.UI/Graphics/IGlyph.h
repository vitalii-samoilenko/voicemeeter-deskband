#pragma once

#include "../Traits/IVisible.h"
#include "../Traits/IMovable.h"
#include "../Traits/IScalable.h"

using namespace ::Voicemeeter::DeskBand::UI::Traits;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Graphics {
				class IGlyph : public IVisible, public IMovable, public IScalable {
				public:
					IGlyph(const IGlyph&) = delete;
					IGlyph(IGlyph&&) = delete;

					virtual ~IGlyph() = 0;

					IGlyph& operator=(const IGlyph&) = delete;
					IGlyph& operator=(IGlyph&&) = delete;

				protected:
					IGlyph() = default;
				};
			}
		}
	}
}