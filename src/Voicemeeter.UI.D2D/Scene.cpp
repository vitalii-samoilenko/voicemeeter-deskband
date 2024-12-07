#include <algorithm>
#include <cmath>
#include <vector>

#include "Windows/Wrappers.h"

#include "Scene.h"

#pragma comment(lib, "d2d1")

using namespace ::Voicemeeter::UI::D2D;

Scene::Scene(
	::std::unique_ptr<Trackers::IDirty>& pDirtyTracker,
	::std::unique_ptr<Trackers::IInput>& pInputTracker,
	::std::unique_ptr<Trackers::IFocus>& pFocusTracker,
	::std::unique_ptr<Graphics::Canvas>& pCanvas,
	::std::unique_ptr<IComponent>& pComposition
) : UI::Scene<Graphics::Canvas>{ pDirtyTracker, pInputTracker, pFocusTracker, pCanvas, pComposition }
  , m_first{ true } {

}

void Scene::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	m_pCanvas->get_pD2dDeviceContext()
		->BeginDraw();
	UI::Scene<Graphics::Canvas>::Redraw(point, vertex);
	::Windows::ThrowIfFailed(m_pCanvas->get_pD2dDeviceContext()
		->EndDraw(
	), "Render failed");
	const ::std::valarray<double>& canvasVertex{ m_pCanvas->get_Size() };
	RECT rect{
		static_cast<LONG>(::std::floor(point[0])),
		static_cast<LONG>(::std::floor(point[1])),
		static_cast<LONG>(::std::ceil(::std::min(canvasVertex[0], point[0] + vertex[0]))),
		static_cast<LONG>(::std::ceil(::std::min(canvasVertex[1], point[1] + vertex[1])))
	};
	DXGI_PRESENT_PARAMETERS params{
		1U, &rect,
		nullptr,
		nullptr
	};
	if (m_first) {
		::Windows::ThrowIfFailed(m_pCanvas->get_pDxgiSwapChain()
			->Present(
				1, 0
		), "Presentation failed");
		m_first = false;
	} else {
		::Windows::ThrowIfFailed(m_pCanvas->get_pDxgiSwapChain()
			->Present1(
				1, 0,
				&params
		), "Presentation failed");
	}

}
void Scene::Resize(const ::std::valarray<double>& vertex) {
	::std::valarray<double> patched{
		::std::max(8., vertex[0]),
		::std::max(8., vertex[1])
	};
	UI::Scene<Graphics::Canvas>::Resize(patched);
	m_first = true;
}
void Scene::Rescale(const ::std::valarray<double>& vertex) {
	::std::valarray<double> patched{
		::std::max(8., vertex[0]),
		::std::max(8., vertex[1])
	};
	UI::Scene<Graphics::Canvas>::Rescale(patched);
	m_first = true;
}
void Scene::Redraw() {
	::std::vector<RECT> cRect{};
	if (!m_first) {
		const ::std::valarray<double>& dirtyPoint{ m_pDirtyTracker->get_Position() };
		const ::std::valarray<double>& dirtyVertex{ m_pDirtyTracker->get_Size() };
		const ::std::valarray<double>& canvasVertex{ m_pCanvas->get_Size() };
		for (size_t i{ 0 }; i < dirtyVertex.size(); i += canvasVertex.size()) {
			cRect.push_back({
				static_cast<LONG>(::std::floor(dirtyPoint[i])),
				static_cast<LONG>(::std::floor(dirtyPoint[i + 1])),
				static_cast<LONG>(::std::ceil(::std::min(canvasVertex[0], dirtyPoint[i] + dirtyVertex[i]))),
				static_cast<LONG>(::std::ceil(::std::min(canvasVertex[1], dirtyPoint[i + 1] + dirtyVertex[i + 1])))
			});
		}
	}
	m_pCanvas->get_pD2dDeviceContext()
		->BeginDraw();
	UI::Scene<Graphics::Canvas>::Redraw();
	::Windows::ThrowIfFailed(m_pCanvas->get_pD2dDeviceContext()
		->EndDraw(
	), "Render failed");
	DXGI_PRESENT_PARAMETERS params{
		static_cast<UINT>(cRect.size()), cRect.data(),
		nullptr,
		nullptr
	};
	if (m_first) {
		::Windows::ThrowIfFailed(m_pCanvas->get_pDxgiSwapChain()
			->Present(
				1, 0
		), "Presentation failed");
		m_first = false;
	} else {
		::Windows::ThrowIfFailed(m_pCanvas->get_pDxgiSwapChain()
			->Present1(
				1, 0,
				&params
		), "Presentation failed");
	}
}