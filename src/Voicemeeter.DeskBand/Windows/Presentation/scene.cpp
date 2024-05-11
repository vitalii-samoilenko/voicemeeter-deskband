#include "scene.h"

using namespace Voicemeeter::DeskBand::Windows::Presentation;

void Scene::Initialize(HWND hWnd) {
	m_ctx = m_drwEngine.Initialize(hWnd);

	const DrawingEngine::Manifest& manifest{ m_drwEngine.get_manifest() };
	//m_cLbl.push_back(manifest.get_fMain().Bind(m_ctx, L"A", 100.F, 100.F));
	m_cFrame.push_back(manifest.get_iBtnRound().Bind(m_ctx));
	m_cBrush.push_back(manifest.get_cActive().Bind(m_ctx));
}
void Scene::Resize(UINT w, UINT h) {
	m_ctx.Resize(w, h);

	m_scale = D2D1::Matrix3x2F::Scale(w / 48.F, h / 48.F);
}

void Scene::Draw() {
	m_ctx.BeginDraw();
	m_ctx.SetTransform(m_scale);

	for (DrawingEngine::Context::Text& lbl : m_cLbl) {
		lbl.Draw(m_cBrush.front());
	}
	for (DrawingEngine::Context::Glyph& frame : m_cFrame) {
		frame.Draw(m_cBrush.front());
	}

	m_ctx.SetTransform(D2D1::IdentityMatrix());
	m_ctx.EndDraw();
}