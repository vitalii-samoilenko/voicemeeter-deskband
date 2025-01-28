#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include <cmath>
#include <utility>
#include <vector>

#include "Scene.h"

using namespace ::Voicemeeter::UI::D3D12;

Scene::Scene(
	::std::unique_ptr<Graphics::Palette>& pPalette,
	::std::unique_ptr<UI::Trackers::Input>& pInputTracker,
	::std::unique_ptr<UI::Trackers::Focus>& pFocusTracker,
	::std::unique_ptr<UI::Graphics::ICanvas>& pCanvas,
	::std::unique_ptr<IComponent>& pComposition
) : UI::Scene{ pInputTracker, pFocusTracker, pCanvas, pComposition }
  , m_pPalette{ ::std::move(pPalette) }
  , m_first{ true } {

}

void Scene::Resize(const ::std::valarray<double>& vertex) {
	UI::Scene::Resize(vertex);
	m_first = true;
	m_pPalette->get_Atlas()
		.Rescale(
			get_pComposition()
				->get_Size()
			/ get_pComposition()
				->get_BaseSize());
}
void Scene::Rescale(const ::std::valarray<double>& vertex) {
	UI::Scene::Rescale(vertex);
	m_first = true;
	m_pPalette->get_Atlas()
		.Rescale(
			get_pComposition()
				->get_Size()
			/ get_pComposition()
				->get_BaseSize());
}

void Scene::Render() {
	Graphics::Queue queue{ ::std::move(m_pPalette->get_Queue()) };
	if (queue.Empty()) {
		return;
	}
	m_pPalette->get_Timer()
		.Tick();
	::std::vector<RECT> cRect{};
	const ::std::valarray<double>& canvasVertex{ get_Size() };
	//m_pPalette->get_Instrumentation()
	//	.get_pDeviceContext()
	//		->BeginDraw();
	for (Graphics::Bundle* pBundle : queue) {
		const ::std::valarray<double>& dirtyPoint{ pBundle->get_Position() };
		const ::std::valarray<double>& dirtyVertex{ pBundle->get_Size() };
		cRect.push_back({
			static_cast<LONG>(::std::floor(dirtyPoint[0])),
			static_cast<LONG>(::std::floor(dirtyPoint[01])),
			static_cast<LONG>(::std::ceil(::std::min(canvasVertex[0], dirtyPoint[0] + dirtyVertex[0]))),
			static_cast<LONG>(::std::ceil(::std::min(canvasVertex[1], dirtyPoint[1] + dirtyVertex[1])))
		});
		pBundle->Execute();
	}
	//::Windows::ThrowIfFailed(m_pPalette->get_Instrumentation()
	//	.get_pDeviceContext()
	//		->EndDraw(
	//), "Render failed");
	if (m_first) {
		::Windows::ThrowIfFailed(m_pPalette->get_Instrumentation()
			.get_pSwapChain()
				->Present(
					0U, 0U
		), "Presentation failed");
		m_first = false;
	} else {
		DXGI_PRESENT_PARAMETERS params{
			static_cast<UINT>(cRect.size()), cRect.data(),
			nullptr,
			nullptr
		};
		::Windows::ThrowIfFailed(m_pPalette->get_Instrumentation()
			.get_pSwapChain()
				->Present1(
					0U, 0U,
					&params
		), "Presentation failed");
	}
}