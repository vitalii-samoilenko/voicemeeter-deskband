#include "Windows/Wrappers.h"

#include "Scene.h"

#pragma comment(lib, "d2d1")

using namespace ::Voicemeeter::UI::D2D;

void Scene::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	m_pCanvas->get_pRenderTarget()
		->BeginDraw();

	UI::Scene<Graphics::Canvas>::Redraw(point, vertex);

	::Windows::ThrowIfFailed(m_pCanvas->get_pRenderTarget()
		->EndDraw(
	), "Render failed");
}