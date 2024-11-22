#include <exception>
#include <utility>

#include "Dirty.h"

using namespace ::Voicemeeter::UI::Trackers;

Dirty::Dirty(
	::Environment::IDirtyTracker& envDirtyTracker,
	::Environment::ITimer& envTimer
) : m_envDirtyTracker{ envDirtyTracker }
  , m_point{ 0., 0. }
  , m_vertex{ 0., 0. } {
	envTimer.Set(::std::chrono::milliseconds{ 1000 / 120 },
		[this]()->bool {
			if (!m_cpGlyph.empty()) {
				m_envDirtyTracker.SetDirty();
			}
			return true;
		});
}

const ::std::valarray<double>& Dirty::get_Position() const {
	return m_point;
}
const ::std::valarray<double>& Dirty::get_Size() const {
	return m_vertex;
}
void Dirty::set_Dirty(Graphics::IGlyph& glyph, bool value) {
	if (value) {
		m_cpGlyph.insert(&glyph);
	} else {
		m_cpGlyph.erase(&glyph);
	}
}
void Dirty::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	if (&point != &m_point || &vertex != &m_vertex) {
		throw ::std::exception{ "Redraw must be done using dirty rectangle only" };
	}
	::std::unordered_set<Graphics::IGlyph*> cpGlyph{ ::std::move(m_cpGlyph) };
	for (Graphics::IGlyph* pGlyph : cpGlyph) {
		pGlyph->Redraw(
			pGlyph->get_Position(), 
			pGlyph->get_Size()
		);
	}
}