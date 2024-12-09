#pragma once

#include <unordered_set>

#include "Environment/IDirtyTracker.h"
#include "Environment/ITimer.h"
#include "Voicemeeter.UI/Graphics/IGlyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Trackers {
				class Dirty final {
				public:
					Dirty(
						::Environment::IDirtyTracker& envDirtyTracker,
						::Environment::ITimer& timer
					);
					Dirty() = delete;
					Dirty(const Dirty&) = delete;
					Dirty(Dirty&&) = default;

					~Dirty() = default;

					Dirty& operator=(const Dirty&) = delete;
					Dirty& operator=(Dirty&&) = delete;

					inline void set_Dirty(UI::Graphics::IGlyph& glyph, bool value) {
						if (value) {
							m_cpGlyph.insert(&glyph);
						} else {
							m_cpGlyph.erase(&glyph);
						}
					};

					inline auto begin() {
						return m_cpGlyph.begin();
					};
					inline auto end() {
						return m_cpGlyph.end();
					};

				private:
					::Environment::IDirtyTracker& m_envDirtyTracker;
					::std::unordered_set<UI::Graphics::IGlyph*> m_cpGlyph;
				};
			}
		}
	}
}