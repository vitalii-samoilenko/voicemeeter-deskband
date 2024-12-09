#include "Dirty.h"

using namespace ::Voicemeeter::UI::D2D::Trackers;

Dirty::Dirty(
	::Environment::IDirtyTracker& envDirtyTracker,
	::Environment::ITimer& timer
) : m_envDirtyTracker{ envDirtyTracker } {
	timer.Set(::std::chrono::milliseconds{ 1000 / 120 },
		[this]()->bool {
			if (!m_cpGlyph.empty()) {
				m_envDirtyTracker.SetDirty();
			}
			return true;
		});
}