#include <utility>

#include "Scene.h"

using namespace ::Voicemeeter::DeskBand::UI;
using namespace ::Voicemeeter::DeskBand::UI::Graphics;

Scene::Scene(
	IAppMouseTracker& appMouseTracker,
	::std::unique_ptr<IComponent> pComposition,
	::std::unique_ptr<ICanvas> pCanvas
) : m_appMouseTracker{ appMouseTracker }
  , m_pComposition{ ::std::move(pComposition) }
  , m_pCanvas{ ::std::move(pCanvas) }
  , m_pPinned{ nullptr } {

}

void Scene::Redraw(const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) {
	m_pCanvas->Redraw(point, vertex);
	m_pComposition->Redraw(point, vertex);
}
void Scene::Resize(const ::linear_algebra::vector& vertex) {
	m_pCanvas->Resize(vertex);
	m_pComposition->Rescale(vertex);
}
bool Scene::MouseLDown(const ::linear_algebra::vector& point) {
	return m_pComposition->MouseLDown(point);
}
bool Scene::MouseRDown(const ::linear_algebra::vector& point) {
	return m_pComposition->MouseRDown(point);
}
bool Scene::MouseWheel(const ::linear_algebra::vector& point, int delta) {
	return m_pComposition->MouseWheel(point, delta);
}
void Scene::MouseMove(const ::linear_algebra::vector& point) {
	m_pPinned->MouseMove(point);
}
void Scene::MouseLUp(const ::linear_algebra::vector& point) {
	m_pPinned->MouseLUp(point);
}
void Scene::EnableMouseTrack(IComponent& pComponent) {
	m_pPinned = &pComponent;

	m_appMouseTracker.EnableMouseTrack();
}
void Scene::DisableMouseTrack(IComponent& pComponent) {
	m_appMouseTracker.DisableMouseTrack();

	m_pPinned = nullptr;
}