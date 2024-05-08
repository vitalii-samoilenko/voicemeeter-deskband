#include "scene.h"

using namespace Voicemeeter::DeskBand::Windows::Presentation;

Scene::Scene(
	Window& rWnd,
	DrawingEngine& rDrwEngine,
	const std::vector<DrawingEngine::resource_type>& resources
) : m_rWnd{ rWnd }
  , m_rDrwEngine { rDrwEngine }
  , m_resources{ resources }
  , m_snapshot{} {

	}

void Scene::Initialize() {
	m_snapshot = m_rDrwEngine.Initialize(
		m_rWnd.get_hWnd(),
		m_resources
	);
}

void Scene::Resize(UINT w, UINT h) {
	m_snapshot.get_pTarget()
		->Resize(D2D1::SizeU(w, h));
}