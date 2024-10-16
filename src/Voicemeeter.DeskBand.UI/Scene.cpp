#include "Scene.h"

using namespace Voicemeeter::DeskBand::UI;
using namespace Voicemeeter::DeskBand::UI::Graphics;

Scene::Scene(
	IAppMouseTracker* pAppMouseTracker,
	IComponent* pComposition,
	ICanvas* pCanvas
) : m_pAppMouseTracker{ pAppMouseTracker }
  , m_pComposition{ pComposition }
  , m_pCanvas{ pCanvas }
  , m_pPinned{ nullptr } {

}

const linear_algebra::vector Scene::get_Position() {
	return m_pCanvas->get_Position();
}
const linear_algebra::vector Scene::get_Size() {
	return m_pCanvas->get_Size();
}

void Scene::Redraw(linear_algebra::vector point, linear_algebra::vector vertex) {
	m_pCanvas->Redraw(point, vertex);
	m_pComposition->Redraw(point, vertex);
}

void Scene::Resize(linear_algebra::vector vertex) {
	m_pCanvas->Resize(vertex);
	m_pComposition->Rescale(vertex);
}

bool Scene::MouseLDown(linear_algebra::vector point) {
	return m_pComposition->MouseLDown(point);
}

bool Scene::MouseRDown(linear_algebra::vector point) {
	return m_pComposition->MouseRDown(point);
}

bool Scene::MouseWheel(linear_algebra::vector point, int delta) {
	return m_pComposition->MouseWheel(point, delta);
}

void Scene::MouseMove(linear_algebra::vector point) {
	m_pPinned->MouseMove(point);
}

void Scene::MouseLUp(linear_algebra::vector point) {
	m_pPinned->MouseLUp(point);
}

void Scene::EnableMouseTrack(IComponent* pComponent) {
	m_pPinned = pComponent;

	m_pAppMouseTracker->EnableMouseTrack();
}

void Scene::DisableMouseTrack(IComponent* pComponent) {
	m_pAppMouseTracker->DisableMouseTrack();

	m_pPinned = nullptr;
}