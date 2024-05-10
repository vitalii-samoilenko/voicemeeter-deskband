#include "drawingengine.h"

#include "../wrappers.h"

using namespace Voicemeeter::DeskBand::Windows::Presentation;

void DrawingEngine::Context::BeginDraw() {
	m_pCtxDevD2->BeginDraw();
}
void DrawingEngine::Context::EndDraw() {
	ThrowIfFailed(m_pCtxDevD2->EndDraw(

	), "Drawing failure");
	DXGI_PRESENT_PARAMETERS params{
		0, NULL,
		NULL, NULL
	};
	ThrowIfFailed(m_pSwChDx->Present1(
		1, 0,
		&params
	), "Presentation failure");
}
void DrawingEngine::Context::Resize(UINT w, UINT h) {
	m_pSwChDx->ResizeBuffers1(
		0,
		w, h,
		DXGI_FORMAT_UNKNOWN,
		0,
		NULL, NULL
	);
}