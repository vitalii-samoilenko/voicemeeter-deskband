#include <exception>
#include <tuple>
#include <utility>

#include "Dirty.h"

using namespace ::Voicemeeter::UI::Trackers;

Dirty::Dirty(
	::Environment::IDirtyTracker& envDirtyTracker,
	::Environment::ITimer& envTimer
) : m_envDirtyTracker{ envDirtyTracker }
  , m_point{ 0., 0. }
  , m_vertex{ 0., 0. }
  , m_changed{ false }{
	envTimer.Set(::std::chrono::milliseconds{ 1000 / 120 },
		[this]()->bool {
			if (!m_cpGlyph.empty()) {
				m_envDirtyTracker.SetDirty();
			}
			return true;
		});
}

const ::std::valarray<double>& Dirty::get_Position() const {
	if (m_changed) {
		Adjust();
	}
	return m_point;
}
const ::std::valarray<double>& Dirty::get_Size() const {
	if (m_changed) {
		Adjust();
	}
	return m_vertex;
}
void Dirty::set_Dirty(Graphics::IGlyph& glyph, bool value) {
	if (value) {
		m_changed = m_cpGlyph.insert(&glyph).second
			|| m_changed;
	} else {
		m_changed = m_cpGlyph.erase(&glyph)
			|| m_changed;
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

void Dirty::Adjust() const {
	if (m_cpGlyph.empty()) {
		m_point = { 0., 0. };
		m_vertex = { 0., 0. };
	} else {
		size_t dimension{
			(*m_cpGlyph.begin())
				->get_Position()
					.size()
		};
		size_t totalSize{ m_cpGlyph.size() * dimension };
		m_point.resize(totalSize);
		m_vertex.resize(totalSize);
		size_t i{ 0 };
		for (const Graphics::IGlyph* pGlyph : m_cpGlyph) {
			::std::slice current{ i, dimension, 1 };
			m_point[current] = pGlyph->get_Position();
			m_vertex[current] = pGlyph->get_Size();
			i += dimension;
		}
	}
}