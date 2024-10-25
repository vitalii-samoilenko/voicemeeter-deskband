#include <utility>

#include "Scene.h"

using namespace ::Voicemeeter::DeskBand::UI;
using namespace ::Voicemeeter::DeskBand::UI::Graphics;

Scene::Scene(
	::std::unique_ptr<IInputTracker> inputTracker,
	::std::unique_ptr<ICanvas> pCanvas,
	::std::unique_ptr<IComponent> pComposition
) : m_inputTracker{ ::std::move(inputTracker) }
  , m_pCanvas{ ::std::move(pCanvas) }
  , m_pComposition{ ::std::move(pComposition) } {

}

const ::linear_algebra::vectord& Scene::get_Position() const {
	return m_pCanvas->get_Position();
}
const ::linear_algebra::vectord& Scene::get_Size() const {
	return m_pCanvas->get_Size();
}

void Scene::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	m_pCanvas->Redraw(point, vertex);
	m_pComposition->Redraw(point, vertex);
}
void Scene::Resize(const ::linear_algebra::vectord& vertex) {
	m_pCanvas->Resize(vertex);
	m_pComposition->Rescale(vertex);
}
bool Scene::MouseLDown(const ::linear_algebra::vectord& point) {
	return m_inputTracker->MouseLDown(point)
		|| m_pComposition->MouseLDown(point);
}
bool Scene::MouseLDouble(const ::linear_algebra::vectord& point) {
	return m_inputTracker->MouseLDouble(point)
		|| m_pComposition->MouseLDouble(point);
}
bool Scene::MouseRDown(const ::linear_algebra::vectord& point) {
	return m_inputTracker->MouseRDown(point)
		|| m_pComposition->MouseRDown(point);
}
bool Scene::MouseWheel(const ::linear_algebra::vectord& point, int delta) {
	return m_inputTracker->MouseWheel(point, delta)
		|| m_pComposition->MouseWheel(point, delta);
}
bool Scene::MouseMove(const ::linear_algebra::vectord& point) {
	return m_inputTracker->MouseMove(point)
		|| m_pComposition->MouseMove(point);
}
bool Scene::MouseLUp(const ::linear_algebra::vectord& point) {
	return m_inputTracker->MouseLUp(point)
		|| m_pComposition->MouseLUp(point);
}