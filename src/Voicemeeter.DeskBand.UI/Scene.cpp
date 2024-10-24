#include <utility>

#include "Scene.h"

using namespace ::Voicemeeter::DeskBand::UI;
using namespace ::Voicemeeter::DeskBand::UI::Graphics;

Scene::Scene(
	IAppMouseTracker& appMouseTracker
) : m_appMouseTracker{ appMouseTracker }
  , m_pCanvas{ nullptr }
  , m_pComposition{ nullptr }
  , m_pPinned{ nullptr } {

}

const ::linear_algebra::vector& Scene::get_Position() const {
	return m_pCanvas->get_Position();
}
const ::linear_algebra::vector& Scene::get_Size() const {
	return m_pCanvas->get_Size();
}
void Scene::set_Canvas(::std::unique_ptr<ICanvas> pCanvas) {
	m_pCanvas.swap(pCanvas);
}
void Scene::set_Composition(::std::unique_ptr<IComponent> pComposition) {
	m_pComposition.swap(pComposition);
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
	if (!m_pPinned) {
		return;
	}

	m_pPinned->MouseMove(point);
}
void Scene::MouseLUp(const ::linear_algebra::vector& point) {
	if (!m_pPinned) {
		return;
	}

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