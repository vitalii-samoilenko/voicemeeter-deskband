#include "drawingengine.h"

#include "../wrappers.h"

using namespace Voicemeeter::DeskBand::Windows::Presentation;

DrawingEngine::DrawingEngine(const Style& style)
	: m_manifest{}
	, m_pFactory{ nullptr }
	, m_pDevice{ nullptr } {
	UINT flags{ 0U };

#ifndef NDEBUG
	ComPtr<ID3D12Debug3> pDebug{ nullptr };
	ThrowIfFailed(D3D12GetDebugInterface(
		IID_PPV_ARGS(&pDebug)
	), "Could not get debug interface");

	pDebug->EnableDebugLayer();

	flags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	
	ThrowIfFailed(CreateDXGIFactory2(
		flags,
		IID_PPV_ARGS(&m_pFactory)
	), "DXGI factory creation failed");

	ThrowIfFailed(D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_12_1,
		IID_PPV_ARGS(&m_pDevice)
	), "D3D12 device creation failed");
}

std::unique_ptr<DrawingEngine::Context> DrawingEngine::Initialize(HWND hWnd, IDCompositionVisual* pCompVisual) {
	return std::unique_ptr<DrawingEngine::Context>{ new DrawingEngine::Context{
		m_pFactory.Get(), m_pDevice.Get(), hWnd, pCompVisual
	} };
}