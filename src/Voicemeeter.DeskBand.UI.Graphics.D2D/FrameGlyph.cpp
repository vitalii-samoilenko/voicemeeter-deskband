#include "FrameGlyph.h"

using namespace ::Voicemeeter::DeskBand::UI::Graphics::D2D;

::linear_algebra::vector FrameGlyph::get_Position() const {
	return m_cpGlyph[m_frame]->get_Position();
}
::linear_algebra::vector FrameGlyph::get_Size() const {
	return m_cpGlyph[m_frame]->get_Size();
}
::linear_algebra::vector FrameGlyph::get_BaseSize() const {
	return m_cpGlyph[m_frame]->get_BaseSize();
}

void FrameGlyph::Redraw(::linear_algebra::vector point, ::linear_algebra::vector vertex) {
	m_cpGlyph[m_frame]->Redraw(point, vertex);
}
void FrameGlyph::Move(::linear_algebra::vector point) {
	for (const ::std::unique_ptr<IGlyph>& pGlyph : m_cpGlyph) {
		pGlyph->Move(point);
	}
}
void FrameGlyph::Rescale(::linear_algebra::vector vertex) {
	for (const ::std::unique_ptr<IGlyph>& pGlyph : m_cpGlyph) {
		pGlyph->Rescale(vertex);
	}
}