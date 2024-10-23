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

::linear_algebra::vector Scene::get_Position() const {
	return m_pCanvas->get_Position();
}
::linear_algebra::vector Scene::get_Size() const {
	return m_pCanvas->get_Size();
}

void Scene::Redraw(::linear_algebra::vector point, ::linear_algebra::vector vertex) {
	m_pCanvas->Redraw(point, vertex);
	m_pComposition->Redraw(point, vertex);
}
void Scene::Resize(::linear_algebra::vector vertex) {
	m_pCanvas->Resize(vertex);
	m_pComposition->Rescale(vertex);
}
bool Scene::MouseLDown(::linear_algebra::vector point) {
	return m_pComposition->MouseLDown(point);
}
bool Scene::MouseRDown(::linear_algebra::vector point) {
	return m_pComposition->MouseRDown(point);
}
bool Scene::MouseWheel(::linear_algebra::vector point, int delta) {
	return m_pComposition->MouseWheel(point, delta);
}
void Scene::MouseMove(::linear_algebra::vector point) {
	m_pPinned->MouseMove(point);
}
void Scene::MouseLUp(::linear_algebra::vector point) {
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