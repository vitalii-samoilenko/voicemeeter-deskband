#include "scene.h"

using namespace Voicemeeter::DeskBand::Windows::Presentation;

void Scene::Initialize(HWND hWnd, IDCompositionVisual* pCompVisual) {
	m_pCtx = m_drwEngine.Initialize(hWnd, pCompVisual);
}
void Scene::Resize(UINT w, UINT h) const {
	m_pCtx->Resize(w, h);
	m_pCtx->Render();
	m_pCtx->Update();
}