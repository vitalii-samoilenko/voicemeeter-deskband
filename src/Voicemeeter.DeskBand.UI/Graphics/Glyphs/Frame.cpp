#include "Frame.h"

using namespace ::Voicemeeter::DeskBand::UI::Graphics::Glyphs;

const ::linear_algebra::vectord& Frame::get_Position() const {
	return m_cpGlyph[m_frame]->get_Position();
}
const ::linear_algebra::vectord& Frame::get_Size() const {
	return m_cpGlyph[m_frame]->get_Size();
}
const ::linear_algebra::vectord& Frame::get_BaseSize() const {
	return m_cpGlyph[m_frame]->get_BaseSize();
}

void Frame::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	m_cpGlyph[m_frame]->Redraw(point, vertex);
}
void Frame::Move(const ::linear_algebra::vectord& point) {
	for (const ::std::unique_ptr<IGlyph>& pGlyph : m_cpGlyph) {
		pGlyph->Move(point);
	}
}
void Frame::Rescale(const ::linear_algebra::vectord& vertex) {
	for (const ::std::unique_ptr<IGlyph>& pGlyph : m_cpGlyph) {
		pGlyph->Rescale(vertex);
	}
}