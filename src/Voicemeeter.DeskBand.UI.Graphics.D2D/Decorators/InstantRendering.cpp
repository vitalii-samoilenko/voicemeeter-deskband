#include <utility>

#include <d2d1_3.h>
#pragma comment(lib, "d2d1")

#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "InstantRendering.h"

using namespace ::Voicemeeter::DeskBand::Windows;
using namespace ::Voicemeeter::DeskBand::UI::Graphics::D2D;

InstantRendering::InstantRendering(
	Canvas& canvas,
	::std::unique_ptr<IGlyph> pGlyph
) : m_canvas{ canvas }
  , m_pGlyph{ ::std::move(pGlyph) } {

}

const ::linear_algebra::vectord& InstantRendering::get_Position() const {
	return m_pGlyph->get_Position();
}
const ::linear_algebra::vectord& InstantRendering::get_Size() const {
	return m_pGlyph->get_Size();
}
const ::linear_algebra::vectord& InstantRendering::get_BaseSize() const {
	return m_pGlyph->get_BaseSize();
}

void InstantRendering::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	ID2D1HwndRenderTarget* pRenderTarget{ m_canvas.get_pRenderTarget() };

	pRenderTarget->BeginDraw();

	m_pGlyph->Redraw(point, vertex);

	ThrowIfFailed(pRenderTarget->EndDraw(
	), "Render failed");
}
void InstantRendering::Move(const ::linear_algebra::vectord& point) {
	m_pGlyph->Move(point);
}
void InstantRendering::Rescale(const ::linear_algebra::vectord& vertex) {
	m_pGlyph->Rescale(vertex);
}