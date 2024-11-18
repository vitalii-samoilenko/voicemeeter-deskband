#include "Frame.h"

using namespace ::Voicemeeter::UI::Graphics::Glyphs;

const ::std::valarray<double>& Frame::get_Position() const {
	return m_cpGlyph[m_frame]->get_Position();
}
const ::std::valarray<double>& Frame::get_Size() const {
	return m_cpGlyph[m_frame]->get_Size();
}
const ::std::valarray<double>& Frame::get_BaseSize() const {
	return m_cpGlyph[m_frame]->get_BaseSize();
}

void Frame::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	m_cpGlyph[m_frame]->Redraw(point, vertex);
}
void Frame::Move(const ::std::valarray<double>& point) {
	for (const ::std::unique_ptr<IGlyph>& pGlyph : m_cpGlyph) {
		pGlyph->Move(point);
	}
}
void Frame::Rescale(const ::std::valarray<double>& vertex) {
	for (const ::std::unique_ptr<IGlyph>& pGlyph : m_cpGlyph) {
		pGlyph->Rescale(vertex);
	}
}