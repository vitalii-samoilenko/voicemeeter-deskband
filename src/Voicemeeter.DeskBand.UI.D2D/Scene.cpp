#include "Scene.h"

using namespace ::Voicemeeter::DeskBand::UI::D2D;

void Scene::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	m_pCanvas->get_pRenderTarget()
		->BeginDraw();

	UI::Scene<Graphics::Canvas>::Redraw(point, vertex);

	ThrowIfFailed(m_pCanvas->get_pRenderTarget()
		->EndDraw(
	), "Render failed");
}