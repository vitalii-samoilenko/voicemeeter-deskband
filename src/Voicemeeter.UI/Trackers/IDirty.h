#pragma once

#include "../Graphics/IGlyph.h"
#include "../Traits/IVisible.h"

namespace Voicemeeter {
	namespace UI {
		namespace Trackers {
			class IDirty : public Traits::IVisible {
			public:
				IDirty(const IDirty&) = delete;
				IDirty(IDirty&&) = delete;

				virtual ~IDirty() = default;

				IDirty& operator=(const IDirty&) = delete;
				IDirty& operator=(IDirty&&) = delete;

				virtual void set_Dirty(Graphics::IGlyph& glyph, bool value) = 0;

			protected:
				IDirty() = default;
			};
		}
	}
}