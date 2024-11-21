#include <algorithm>
#include <cmath>
#include <vector>

#include "Windows/Wrappers.h"

#include "Scene.h"

#pragma comment(lib, "d2d1")

using namespace ::Voicemeeter::UI::D2D;

Scene::Scene(
	::std::unique_ptr<Trackers::Dirty>& pDirtyTracker,
	::std::unique_ptr<Trackers::IInput>& pInputTracker,
	::std::unique_ptr<Trackers::IFocus>& pFocusTracker,
	::std::unique_ptr<Graphics::Canvas>& pCanvas,
	::std::unique_ptr<IComponent>& pComposition
) : UI::Scene<Graphics::Canvas, Trackers::Dirty>{ pDirtyTracker, pInputTracker, pFocusTracker, pCanvas, pComposition }
  , m_first{ false } {

}

void Scene::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	m_pCanvas->get_pD2dDeviceContext()
		->BeginDraw();
	UI::Scene<Graphics::Canvas, Trackers::Dirty>::Redraw(point, vertex);
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
	UI::Scene<Graphics::Canvas, Trackers::Dirty>::Resize(patched);
	m_first = true;
}
void Scene::Redraw() {
	const ::std::valarray<double>& cVertex{ m_pCanvas->get_Size() };
	::std::vector<RECT> cRect{};
	for (const IGlyph* pGlyph : *m_pDirtyTracker) {
		const ::std::valarray<double>& point{ pGlyph->get_Position() };
		const ::std::valarray<double>& vertex{ pGlyph->get_Size() };
		cRect.push_back({
			static_cast<LONG>(::std::floor(point[0])),
			static_cast<LONG>(::std::floor(point[1])),
			static_cast<LONG>(::std::ceil(::std::min(cVertex[0], point[0] + vertex[0]))),
			static_cast<LONG>(::std::ceil(::std::min(cVertex[1], point[1] + vertex[1])))
		});
	}
	m_pCanvas->get_pD2dDeviceContext()
		->BeginDraw();
	UI::Scene<Graphics::Canvas, Trackers::Dirty>::Redraw();
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