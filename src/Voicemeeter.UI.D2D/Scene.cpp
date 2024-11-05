#include <algorithm>
#include <cmath>

#include "Windows/Wrappers.h"

#include "Scene.h"

#pragma comment(lib, "d2d1")

using namespace ::Voicemeeter::UI::D2D;

void Scene::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	m_pCanvas->get_pD2dDeviceContext()
		->BeginDraw();

	UI::Scene<Graphics::Canvas>::Redraw(point, vertex);

	::Windows::ThrowIfFailed(m_pCanvas->get_pD2dDeviceContext()
		->EndDraw(
	), "Render failed");

	RECT rect{
		static_cast<LONG>(::std::floor(point.x)),
		static_cast<LONG>(::std::floor(point.y)),
		static_cast<LONG>(::std::ceil(point.x + vertex.x)),
		static_cast<LONG>(::std::ceil(point.y + vertex.y))
	};
	DXGI_PRESENT_PARAMETERS params{
		1U, &rect,
		nullptr,
		nullptr
	};
	::Windows::ThrowIfFailed(m_pCanvas->get_pDxgiSwapChain()
		->Present1(
			1, 0,
			&params
	), "Presentation failed");
}
void Scene::Resize(const ::linear_algebra::vectord& vertex) {
	::linear_algebra::vectord patched{
		::std::max(8., vertex.x),
		::std::max(8., vertex.y)
	};
	UI::Scene<Graphics::Canvas>::Resize(patched);
}