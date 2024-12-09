#include <algorithm>
#include <cmath>
#include <vector>

#include "Windows/Wrappers.h"

#include "Scene.h"

#pragma comment(lib, "d2d1")

using namespace ::Voicemeeter::UI::D2D;

Scene::Scene(
	::std::unique_ptr<Graphics::Palette>& pPalette,
	::std::unique_ptr<Trackers::Dirty>& pDirtyTracker,
	::std::unique_ptr<UI::Trackers::Input>& pInputTracker,
	::std::unique_ptr<UI::Trackers::Focus>& pFocusTracker,
	::std::unique_ptr<UI::Graphics::ICanvas>& pCanvas,
	::std::unique_ptr<IComponent>& pComposition
) : UI::Scene{ pInputTracker, pFocusTracker, pCanvas, pComposition }
  , m_pPalette{ ::std::move(pPalette) }
  , m_pDirtyTracker{ ::std::move(pDirtyTracker) }
  , m_first{ true } {

}

void Scene::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	m_pPalette->get_pD2dDeviceContext()
		->BeginDraw();
	UI::Scene::Redraw(point, vertex);
	::Windows::ThrowIfFailed(m_pPalette
		->get_pD2dDeviceContext()
			->EndDraw(
	), "Render failed");
	if (m_first) {
		::Windows::ThrowIfFailed(m_pPalette
			->get_pDxgiSwapChain()
				->Present(
					1, 0
		), "Presentation failed");
		m_first = false;
	} else {
		const ::std::valarray<double>& canvasVertex{ get_Size() };
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
		::Windows::ThrowIfFailed(m_pPalette
			->get_pDxgiSwapChain()
				->Present1(
					1, 0,
					&params
		), "Presentation failed");
	}
}
void Scene::Resize(const ::std::valarray<double>& vertex) {
	UI::Scene::Resize(vertex);
	m_first = true;
}
void Scene::Rescale(const ::std::valarray<double>& vertex) {
	UI::Scene::Rescale(vertex);
	m_first = true;
}
void Scene::Redraw() {
	Trackers::Dirty dirtyTracker{ ::std::move(*m_pDirtyTracker) };
	::std::vector<RECT> cRect{};
	const ::std::valarray<double>& canvasVertex{ get_Size() };
	m_pPalette->get_pD2dDeviceContext()
		->BeginDraw();
	for (UI::Graphics::IGlyph* pGlyph : dirtyTracker) {
		const ::std::valarray<double>& dirtyPoint{ pGlyph->get_Position() };
		const ::std::valarray<double>& dirtyVertex{ pGlyph->get_Size() };
		cRect.push_back({
			static_cast<LONG>(::std::floor(dirtyPoint[0])),
			static_cast<LONG>(::std::floor(dirtyPoint[01])),
			static_cast<LONG>(::std::ceil(::std::min(canvasVertex[0], dirtyPoint[0] + dirtyVertex[0]))),
			static_cast<LONG>(::std::ceil(::std::min(canvasVertex[1], dirtyPoint[1] + dirtyVertex[1])))
		});
		pGlyph->Redraw(dirtyPoint, dirtyVertex);
	}
	::Windows::ThrowIfFailed(m_pPalette
		->get_pD2dDeviceContext()
			->EndDraw(
	), "Render failed");
	if (m_first) {
		::Windows::ThrowIfFailed(m_pPalette
			->get_pDxgiSwapChain()
				->Present(
					1, 0
		), "Presentation failed");
		m_first = false;
	} else {
		DXGI_PRESENT_PARAMETERS params{
			static_cast<UINT>(cRect.size()), cRect.data(),
			nullptr,
			nullptr
		};
		::Windows::ThrowIfFailed(m_pPalette
			->get_pDxgiSwapChain()
				->Present1(
					1, 0,
					&params
		), "Presentation failed");
	}
}