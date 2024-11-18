#include <algorithm>
#include <cmath>

#include "Windows/Wrappers.h"

#include "Scene.h"

#pragma comment(lib, "d2d1")

using namespace ::Voicemeeter::UI::D2D;

void Scene::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	m_pCanvas->get_pD2dDeviceContext()
		->BeginDraw();
	UI::Scene<Graphics::Canvas>::Redraw(point, vertex);
	::Windows::ThrowIfFailed(m_pCanvas->get_pD2dDeviceContext()
		->EndDraw(
	), "Render failed");
	RECT rect{
		static_cast<LONG>(::std::floor(point[0])),
		static_cast<LONG>(::std::floor(point[1])),
		static_cast<LONG>(::std::ceil(point[0] + vertex[0])),
		static_cast<LONG>(::std::ceil(point[1] + vertex[1]))
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
void Scene::Resize(const ::std::valarray<double>& vertex) {
	::std::valarray<double> patched{
		::std::max(8., vertex[0]),
		::std::max(8., vertex[1])
	};
	UI::Scene<Graphics::Canvas>::Resize(patched);
}