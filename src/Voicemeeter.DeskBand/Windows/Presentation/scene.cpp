#include "scene.h"

using namespace Voicemeeter::DeskBand::Windows::Presentation;

void Scene::Initialize(HWND hWnd) {
	m_pCtx = m_drwEngine.Initialize(hWnd);
}
void Scene::Resize(UINT w, UINT h) const {
	m_pCtx->Resize(w, h);
}

void Scene::Draw() const {
	m_pCtx->Render();
	m_pCtx->Update();
}