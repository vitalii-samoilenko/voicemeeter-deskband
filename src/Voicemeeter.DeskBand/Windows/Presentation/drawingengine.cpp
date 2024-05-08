#include "drawingengine.h"

#include "../wrappers.h"
#include "../../messages.h"

using namespace Voicemeeter::DeskBand::Windows::Presentation;

DrawingEngine::DrawingEngine(
	Style style
) : m_style{ style }
  , m_pFactory{ NULL }
  , m_pDwFactory{ NULL } {
		ThrowIfFailed(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&m_pFactory
		), "Factory creation failed");
		ThrowIfFailed(DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDwFactory)
		), "DirectWrite factory creation failed");
	}

DrawingEngine::Snapshot DrawingEngine::Initialize(
	HWND hWnd,
	const std::vector<resource_type>& resources
) {
	Snapshot snapshot{};

	ThrowIfFailed(m_pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU()),
		&snapshot.m_pTarget
	), "Render target creation failed");

	for (const resource_type resource : resources) {
		snapshot.m_resources.push_back(NULL);

		switch (resource) {
		case resource_type::bh_sub: {
			CComPtr<ID2D1SolidColorBrush> pBrush{ NULL };
			ThrowIfFailed(snapshot.m_pTarget->CreateSolidColorBrush(
				m_style.Sub,
				&pBrush
			), "Sub brush creation failed");
			snapshot.m_resources.back() = pBrush;
		} break;

		case resource_type::bh_active: {
			CComPtr<ID2D1SolidColorBrush> pBrush{ NULL };
			ThrowIfFailed(snapshot.m_pTarget->CreateSolidColorBrush(
				m_style.Active,
				&pBrush
			), "Active brush creation failed");
			snapshot.m_resources.back() = pBrush;
		} break;

		default:
			throw windows_error{ MSG_ERR_GENERAL, "Invalid resource type" };
		}
	}

	return snapshot;
}