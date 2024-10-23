#include "FrameGlyph.h"

using namespace ::Voicemeeter::DeskBand::UI::Graphics::D2D;

const ::linear_algebra::vector& FrameGlyph::get_Position() const {
	return m_cpGlyph[m_frame]->get_Position();
}
const ::linear_algebra::vector& FrameGlyph::get_Size() const {
	return m_cpGlyph[m_frame]->get_Size();
}
const ::linear_algebra::vector& FrameGlyph::get_BaseSize() const {
	return m_cpGlyph[m_frame]->get_BaseSize();
}

void FrameGlyph::Redraw(const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) {
	m_cpGlyph[m_frame]->Redraw(point, vertex);
}
void FrameGlyph::Move(const ::linear_algebra::vector& point) {
	for (const ::std::unique_ptr<IGlyph>& pGlyph : m_cpGlyph) {
		pGlyph->Move(point);
	}
}
void FrameGlyph::Rescale(const ::linear_algebra::vector& vertex) {
	for (const ::std::unique_ptr<IGlyph>& pGlyph : m_cpGlyph) {
		pGlyph->Rescale(vertex);
	}
}