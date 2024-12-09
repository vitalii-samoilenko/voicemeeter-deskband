#include <utility>

#include "Scene.h"

using namespace ::Voicemeeter::UI;

Scene::Scene(
	::std::unique_ptr<Trackers::Input>& pInputTracker,
	::std::unique_ptr<Trackers::Focus>& pFocusTracker,
	::std::unique_ptr<Graphics::ICanvas>& pCanvas,
	::std::unique_ptr<IComponent>& pComposition
) : m_pInputTracker{ ::std::move(pInputTracker) }
  , m_pFocusTracker{ ::std::move(pFocusTracker) }
  , m_pCanvas{ ::std::move(pCanvas) }
  , m_pComposition{ ::std::move(pComposition) } {

};

void Scene::set_Focus(bool value) {
	m_pInputTracker->set_Focus(value);
	m_pFocusTracker->set_Focus(value);
}
const ::std::valarray<double>& Scene::get_Position() const {
	return m_pCanvas->get_Position();
}
const ::std::valarray<double>& Scene::get_Size() const {
	return m_pCanvas->get_Size();
}
const ::std::valarray<double>& Scene::get_BaseSize() const {
	return m_pComposition->get_BaseSize();
}

void Scene::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	m_pComposition->Redraw(point, vertex);
}
void Scene::Resize(const ::std::valarray<double>& vertex) {
	m_pCanvas->Resize(vertex);
	m_pComposition->Rescale(vertex);
}
void Scene::Rescale(const ::std::valarray<double>& vertex) {
	m_pComposition->Rescale(vertex);
	m_pCanvas->Resize(m_pComposition->get_Size());
}
bool Scene::MouseLDown(const ::std::valarray<double>& point) {
	return m_pInputTracker->MouseLDown(point)
		|| m_pFocusTracker->MouseLDown(point)
		|| m_pComposition->MouseLDown(point);
}
bool Scene::MouseLDouble(const ::std::valarray<double>& point) {
	return m_pInputTracker->MouseLDouble(point)
		|| m_pFocusTracker->MouseLDouble(point)
		|| m_pComposition->MouseLDouble(point);
}
bool Scene::MouseMDown(const ::std::valarray<double>& point) {
	return m_pInputTracker->MouseMDown(point)
		|| m_pFocusTracker->MouseMDown(point)
		|| m_pComposition->MouseMDown(point);
}
bool Scene::MouseMDouble(const ::std::valarray<double>& point) {
	return m_pInputTracker->MouseMDouble(point)
		|| m_pFocusTracker->MouseMDouble(point)
		|| m_pComposition->MouseMDouble(point);
}
bool Scene::MouseRDown(const ::std::valarray<double>& point) {
	return m_pInputTracker->MouseRDown(point)
		|| m_pFocusTracker->MouseRDown(point)
		|| m_pComposition->MouseRDown(point);
}
bool Scene::MouseRDouble(const ::std::valarray<double>& point) {
	return m_pInputTracker->MouseRDouble(point)
		|| m_pFocusTracker->MouseRDouble(point)
		|| m_pComposition->MouseRDouble(point);
}
bool Scene::MouseWheel(const ::std::valarray<double>& point, int delta) {
	return m_pInputTracker->MouseWheel(point, delta)
		|| m_pFocusTracker->MouseWheel(point, delta)
		|| m_pComposition->MouseWheel(point, delta);
}
bool Scene::MouseMove(const ::std::valarray<double>& point) {
	return m_pInputTracker->MouseMove(point)
		|| m_pFocusTracker->MouseMove(point)
		|| m_pComposition->MouseMove(point);
}
bool Scene::MouseLUp(const ::std::valarray<double>& point) {
	return m_pInputTracker->MouseLUp(point)
		|| m_pFocusTracker->MouseLUp(point)
		|| m_pComposition->MouseLUp(point);
}